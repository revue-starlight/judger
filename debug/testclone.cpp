#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <alloca.h>
#include <signal.h>
#include <sys/prctl.h>
#include <list>
#include "../src/utils/log.hpp"
using namespace std;


static void init_signal_handler(int signal) {
    if (signal == SIGCHLD) {
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0);
    } else {
        exit(1);
    }
}




static int clone_init_fn(void *) {
    // a dummy init process in new pid namespace
    // intended to be killed via SIGKILL from root pid namespace
    INFO("clone_init_fn");
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    {
        struct sigaction action;
        action.sa_handler = init_signal_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGKILL, &action, NULL);
        sigaction(SIGHUP, &action, NULL);
        sigaction(SIGINT, &action, NULL);
        sigaction(SIGTERM, &action, NULL);
        sigaction(SIGABRT, &action, NULL);
        sigaction(SIGQUIT, &action, NULL);
        sigaction(SIGPIPE, &action, NULL);
        sigaction(SIGALRM, &action, NULL);
        sigaction(SIGCHLD, &action, NULL);
    }

    // close all fds;

    while (1) pause();
    return 0;
}



int main(){
    int stackSize = 1024*4096;
    int pid = clone(clone_init_fn,(void*)((char*)alloca(stackSize)+stackSize),CLONE_NEWPID,NULL);
    
}