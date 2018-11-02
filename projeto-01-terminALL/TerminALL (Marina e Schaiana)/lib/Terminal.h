/*
 * Terminal.h
 *
 *  Created on: Oct 28, 2015
 *      Author: arliones
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <sys/types.h>
#include <map>
#include <string>
#include <vector>
#include "Process.h"

using namespace std;

class Terminal {
public:

    Terminal();
    virtual ~Terminal();
	
    int run();
    pid_t pid();

private:
    //Professor já implementou. Tira os espaços antes e depois dos comandos
    std::string trim(std::string& str);

 	std::vector<string> substring(string str, char esp = ' ');
    //Lista os processos filhos em execução
    void jobs();

    pid_t _my_pid;
    pid_t _parent;
    typedef std::map<pid_t,Process*> ProcessMap;
    ProcessMap _processes; //Só armazena os processos executados com & porque o comando jobs só lista os processos filhos em execução
};

#endif /* TERMINAL_H_ */
