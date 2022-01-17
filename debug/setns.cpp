#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

int clones(void *arg){
    std::cout<<"clone"<<std::endl;
    int *pidd = (int*)arg;
    int pid = *pidd;
    std::string p;
    p = "/proc/";
    
    p = p+std::to_string(pid)+"/ns/pid";
    

    int fd = open(p.c_str(),O_RDONLY);
    setns(fd,CLONE_NEWPID);
    

    return 0;
}

int main(){
    
    int stackSize = 1024*sysconf(_SC_PAGE_SIZE);
    int p = getpid();
    int *pp = &p;
    std::cout<<p<<std::endl;
    int pid = clone(clones,(void*)((char*)alloca(stackSize)+stackSize),CLONE_NEWPID | SIGCHLD,(void*)pp);
    std::cout<<"newpid"<<pid<<std::endl;
    wait(NULL);
    return 0;
}