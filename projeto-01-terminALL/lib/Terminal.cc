#include "Terminal.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <pwd.h>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

using namespace std;

Terminal::Terminal() {
	_processes = std::map<pid_t,Process*>(); //Inicializa o map
}

Terminal::~Terminal()
{
    //...
}

int Terminal::run()
{
    string entrada;
    std::vector<string> retorno;
    int tvet;                                                            //Quantidade de de palavras no comando

    string userName = getpwuid(getuid()) -> pw_name;                     //Busca o nome de usuário
    char pcName[1024];                               
	gethostname(pcName, 1024);                                            //Salva o nome do computador em um vetor de char
    string dir = get_current_dir_name();                                 //Busca o diretório atual

    cout << "" << userName << "@" << pcName << ":"<< "" << dir << "$ ";  //Primeira execução do terminal
    getline(cin, entrada);
    retorno = substring(entrada);
    //cout << "" << retorno[0] <<"\n";

	while (retorno[0].compare("exit") != 0) {     //Loop infinito até o usuário solicitar saída (exit) 
		
		if (retorno[0].compare("jobs") == 0) {
			jobs();
		} else 	if ((retorno.size() > 0) && (retorno.back().compare("&") == 0)) {      //Caso o terminal tenha que ficar em execução
			//cout << "Posssui & no comando" << endl;
			retorno.pop_back();	//Remove o "&" da lista de argumentos
			Process processo = Process(retorno[0], retorno, false);
			_processes.insert (std::pair<pid_t,Process*>(processo.pid(),&processo));
		}
		else {
			//cout << "Não posssui & no comando" << endl;
			Process processo = Process(retorno[0], retorno, true);
		}

		cout << "" << userName << "@" << pcName << ":"<< "" << dir << "$ "; //Exibe terminal
		getline(cin,entrada);                                               //Recebe novo comando      
		retorno = substring(entrada);                                       //Divide comando em um vetor     
    }
}


void Terminal::jobs() {
	vector<pid_t> pidsRemover; //vetor de pids que estão concluídos e serão removidos do map
	for (map<pid_t, Process*>::iterator it=_processes.begin(); it!=_processes.end(); ++it) { //iterador que percorre o map de pids
		string caminho_arquivo ("/proc/" + to_string(it->first) + "/status");	 //caminho onde estão as informações do processo		
		ifstream arquivo(caminho_arquivo.c_str()); //abre o arquivo de status do processo
    
		if (arquivo.is_open()) {
			char nome[256], status[256];
			int i_status;
			string s_nome, s_status;

			arquivo.getline (nome, 256);
			s_nome = nome; //armazena vetor de char em string para facilitar manipulação
			s_nome = s_nome.substr (6, string::npos);

			arquivo.getline (status, 256);
			s_status = status; //armazena vetor de char em string para facilitar manipulação

			int pos_inicio = s_status.find("("); 
			int pos_fim	= s_status.find(")");			
			string status_fim = s_status.substr(pos_inicio + 1, pos_fim - pos_inicio - 1); //Isola o nome do status
			if (status_fim == "zombie") { //Verifica se o status é zombie
				waitpid(it->first, &i_status, 0); //Se for, dá um wait() no pai para atualizar o status do filho para "concluído"
				status_fim = "concluído";
				//_processes.erase(it); não dá para remover do map assim, dá problema com o iterador, por isso fizemos o vetor pidsRemover
				pidsRemover.push_back(it->first); //Coloca o pid do processo concluído no vetor para futura remoção
			}
			cout << it->first << " - " << s_nome << " - status: " << status_fim << endl; //mostra na tela o status do processo

		}
		arquivo.close();
		
	}

	for(int i=0; i < pidsRemover.size(); i++) { 
		_processes.erase(pidsRemover[i]); //Remove o pid do processo
	}
    
}



std::vector<string> Terminal::substring(string str, char esp){ //Recebe uma string e tem como argumento o caractere ' '
    vector<string> ret;                               //Vetor retorno         

	if (str.length() == 0) { 
		ret.push_back(str);
		return ret;
	}

    int inicio = 0;                                   //contador 

    for(int i = 0; i < str.length(); ++i) {           //Adiciona os caracteres de cada comando em cada posição do vetor 
        if(str[i] == esp) {                         
            if(i - inicio != 0)
               ret.push_back(str.substr(inicio, i-inicio));
            inicio = i+1;
        }
    }
    if(str.length() - inicio != 0)
        ret.push_back(str.substr(inicio, inicio - str.length())); 
    return ret;
 }
