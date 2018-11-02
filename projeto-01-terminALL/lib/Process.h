/*
 * Process.h
 *
 *  Created on: Oct 28, 2015
 *      Author: arliones
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <sys/types.h>
#include <string>
#include <vector>

class Process {
public:
    Process(std::string & program, std::vector<std::string> & params, bool block);
    virtual ~Process();

    pid_t pid();
    int status();

private:
    void exec(std::string & program, std::vector<std::string> & params);
    static char * string_to_c_convert(const std::string & s);

private:
    pid_t  _my_pid;
    pid_t  _parent;
    int _status;
};

#endif /* PROCESS_H_ */
