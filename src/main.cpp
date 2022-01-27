#include <csignal>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/socket.h>
#include "utils/allutils.hpp"
#include "utils/log.hpp"
#include "cgroup.hpp"
#include "spawn.hpp"
#include "config.hpp"
using namespace std;
int setCgroup();
int execute();
int cgroupClean();
cg::Cgroup cgroup;
Config config;
int main(){
    setCgroup();
    execute();
    cgroupClean();
    return 0;
}

int setCgroup(){
    int q = 1;
    {// get Config (DEV)
        cgroup.setcpu(50000);
        cgroup.setmem(1024*1024*1024);
	cgroup.setcpu(34);
    }
    if (cgroup.createAll()==-1) return -1;
    if (cgroup.writeAll()==-1) return -1;
    return 1;
}

int clone_init_fn(void *args){
    mount("proc","/proc","proc",0,NULL);
    INFO("init fn");
    std::fstream p;
    fs::path fp;
    fp/="";
    fp/="proc";
    fp/=to_string(getpid());
    fp/="ns";
    fp/="pid";
    INFO("%s",fp.c_str());
    while(1)pause();
    return 0;
}

int clone_main_func(void *args){
    INFO("waiting for cgroup configurations");
    mount("proc","/proc","proc",0,NULL);
    //int pid = getpid();
    //printFD(pid);    
    execl("/bin/bash","/bin/bash",NULL);
    return 1;
}

int execute(){
    ERROR("execute");
    // init pid;
    int stackSize = 512*sysconf(_SC_PAGE_SIZE); // PAGE_SIZE = 4096
    // init a new pidnamespace as pid 1
    pid_t init_pid = clone(clone_init_fn,(void*)((char*)alloca(stackSize)+stackSize),CLONE_NEWPID,NULL);
    fs::path pidnsPath = "/proc"; pidnsPath /= to_string(init_pid); pidnsPath = pidnsPath / "ns" / "pid";
    int pidnsfd = open(pidnsPath.c_str(),O_RDONLY);
    if (pidnsfd < 0){
        ERROR("unable to open %s",pidnsPath.c_str());
    }
    
    if (setns(pidnsfd,CLONE_NEWPID)==-1){
        ERROR("unable to setns");
    }
    
    close(pidnsfd);
    
    SUCCESS("set newly cloned pidns");

    // socket pair start:
    int socks[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,socks);
    
    int main_func_pid = clone(clone_main_func,(void*)((char*)alloca(stackSize)+stackSize),//CLONE_NEWUSER | 
	CLONE_NEWNS | CLONE_NEWIPC | CLONE_NEWUTS | SIGCHLD,NULL);
    int *x;
    waitpid(main_func_pid,x,0);
    return 1;   
}

int cgroupClean(){
    INFO("clean");
    cgroup.getStatus();
    cgroup.clean();
    return 1;
}
