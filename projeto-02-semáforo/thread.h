#ifndef __thread_h
#define __thread_h

#include <pthread.h>
#include <string.h>
#include <stdlib.h>

class Thread
{
public:
    Thread(void * (*entry)(void *), void * arg, int arg_size) {
        _arg = malloc(arg_size);
        memcpy(_arg, arg, arg_size);
        if(pthread_create(&_thread, 0, entry, _arg)) //if fail
            _thread = 0;
    }
    ~Thread() { free(_arg); }

    int join(int * status) {
        void * res;
        if(pthread_join(_thread, &res)) return -1; //fail
        *status = *((int*)res);
        free(res);
        return 0;//success
    }
    friend void exit(int status = 0) {
        void * exit_status = malloc(sizeof(int));
        memcpy(exit_status, &status, sizeof(int));
        pthread_exit(exit_status);
    }
    friend void yield() { pthread_yield(); }

private:
    pthread_t _thread;
    void * _arg;
};

#endif
