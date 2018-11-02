#include "Process.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

Process::Process(string & program, vector<string> & params, bool block)
{
   pid_t pid = fork(); //Cria o processo filho

   if(pid == -1) { // fork falhou
        perror("Erro ao criar o processo");
        exit(-1);
   }
   if(pid > 0) { //Verifica se é o processo pai
	_my_pid = pid;  //Armazena a id do filho
	_parent = getpid();  //Armazena a id do pai
      if(block == true) {  //Caso o comando digitado não tenha o "&", deixa o processo pai em espera, enquanto o filho executa
         wait(&_status);
      }
   }
   if(pid == 0) {  //Caso seja o processo filho, chama o método exec(), para tratar os parâmetros e executar o programa
   exec(program, params);

   }
   
}

Process::~Process() {

}

void Process::exec(std::string & program, std::vector<std::string> & params) {
    char * program_c = string_to_c_convert(program); //Convertendo o comando referente ao programa em vetor de char
    vector<char*> params_c; //Vetor de vetor de char com os parâmetros do comando
    
    for (int i = 0; i < params.size(); i++) {  //Convertendo cada string do params para vetor de char e armazena em params_c
        params_c.push_back(string_to_c_convert(params[i]));
    }

    params_c.push_back(NULL);   //Coloca null na última posição do vetor de vetor de char, conforme manual do exec
    execvp(program_c, &params_c[0]); //Executa o programa
}


pid_t Process::pid() {

return _my_pid;

}

int Process::status() {

return _status;

}


char * Process::string_to_c_convert(const string & s)
{
    char *pc = new char[s.size()+1];
    strcpy(pc, s.c_str());
    return pc;
}
