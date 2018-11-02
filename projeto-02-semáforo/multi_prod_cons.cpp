#include <iostream>
#include <fstream>
#include <uuid/uuid.h>
#include <unistd.h>
#include <errno.h>
#include "mutex.h"
#include "thread.h"
#include "semaphore.h"
 
using namespace std;
 
const int PRODS = 10; //Quantidade de produtores
const int REP = 10; //Quantidade de repetições
const int CONS = 200; //Quantidade de consumidores
const int BUF_SIZE = 25; //Tamanho do buffer
 
Thread * prods[PRODS]; //Thread do produtor
Thread * cons[CONS]; //Thread do consumidor

uuid_t buffer[BUF_SIZE];
static int prod_pos = 0;
static int cons_pos = 0;

int ncons = 0;
 
Semaphore full = Semaphore(BUF_SIZE); //Semáforo 1
Semaphore empty = Semaphore(0); //Semáforo 2

Mutex mutcout = Mutex(); //Mutex para sincronizar os cout
Mutex mutncons = Mutex(); //Mutex para saber quantas vezes foi consumido
Mutex mutp_pos = Mutex(); //Mutex da posição produzida no buffer
Mutex mutc_pos = Mutex(); //Mutex da posição consumida no buffer

bool finished = false;
 
void* producer(void * arg)
{	mutcout.lock();
	cout << "Producer was born!" << endl;
	mutcout.unlock();
 
	int rep = REP;
 	int i;
	char fname[36+1];
 
	while(rep--)
	{
		
		full.p();
		//Utiliza mutex para incrementar e salvar o valor da variável prod_pos na variável i
		mutp_pos.lock();
		if(++prod_pos == BUF_SIZE) prod_pos = 0;
		i = prod_pos;
		mutp_pos.unlock();

		uuid_generate(buffer[i]);
		uuid_unparse(buffer[i], fname);
		 

		string name(fname,sizeof(uuid_t)*2 + 4);
		ofstream file(name.c_str());
		file << name;
		file.close();
		sync(); //Sync para garantir que o disco está sincronizado
		empty.v();
		
	}
 
	exit(REP);
}
 
void* consumer(void* arg)
{	mutcout.lock();
	cout << "Consumer was born!" << endl;
 	mutcout.unlock();
	char fname[36+1];
	int consumed = 0;
 	int i;
 
	while(true)
	{
				

		mutncons.lock();
		//Verifica se já consumiu tudo	
		if(PRODS*REP == ncons) finished = true;
		ncons++;
		mutncons.unlock();
		//Verifica se deve encerrar o consumidor
		if(finished) exit(consumed);

		consumed++;
		//Utiliza mutex para incrementar e salvar o valor da variável cons_pos na variável i
		mutc_pos.lock();
		if(++cons_pos == BUF_SIZE) cons_pos = 0;
		i = cons_pos;
		mutc_pos.unlock();

		empty.p();
		uuid_unparse(buffer[i], fname);
		
		{
			ifstream file(fname);
			if(!file.good()) continue;
			string str;
			file >> str;
			mutcout.lock();
			cout << "Consumed: " << str << endl;
			mutcout.unlock();	
		}
		
 		

		if(remove(fname)) cerr << "Error: " << errno << endl;
		full.v();
	}
 
	exit(consumed);
}
 
int main()
{	mutcout.lock();
    cout << "Massive Producer x Consumer Problem\n";
	mutcout.unlock(); 
    // Create
    for(int i = 0; i < PRODS; i++)
    	prods[i] = new Thread(&producer, 0, 0);
    for(int i = 0; i < CONS; i++)
    	cons[i] = new Thread(&consumer, 0, 0);
 
    // Join
    int status = 0;
    int produced = 0;
    int consumed = 0;
    for(int i = 0; i < PRODS; i++)
    {
    	prods[i]->join(&status);
    	produced += status;
    }
 
   // finished = true;
 
    for(int i = 0; i < CONS; i++)
    {
    	cons[i]->join(&status);
    	consumed += status;
    }
 	mutcout.lock();
    cout << "Total produced: " << produced << endl;
	mutcout.unlock();	
	
	mutcout.lock();
    cout << "Total consumed: " << consumed << endl;
	mutcout.unlock(); 
    return 0;
}
