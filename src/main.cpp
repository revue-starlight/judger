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
cg::Cgroup *cgroup;
Config *config;
int main(int argc,char *argv[]){
    Arg arg(argc,argv);
    config = new Config(arg);
    cgroup = new cg::Cgroup(*config);
    execute();
    cgroupClean();
    return 0;
}


int clone_init_fn(void *args){
    INFO("init fn");
    while(1)pause();
    return 0;
}

int clone_main_func(void *args){
    int *fd = (int*) args;
    INFO("clone_main_func");
    spawn sp;
    sp.pivot_root();
    mount("proc","/proc","proc",NULL,NULL);
    char buf[4];
    unshare(CLONE_NEWUSER);
    setuid(config->getUid());
    setgid(config->getGid());
    recv(*fd,buf,4,0);
    //execl("/root/repos/judger/testfile/sort8.out","/root/repos/judger/testfile/sort8.out",NULL);
    return 0;
}

int execute(){
    cgroup->createAll();
    int socks[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,socks);
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
    // if (cgroup->writeAll()==-1){
    //     ERROR("unable to write");
    // }
    int main_func_pid = clone(clone_main_func,(void*)((char*)alloca(stackSize)+stackSize),//CLONE_NEWUSER |//  CLONE_NEWIPC | CLONE_NEWUTS |
      CLONE_NEWNS | SIGCHLD,socks+1);
    cgroup->bind(main_func_pid,cg::CPU);
    cgroup->bind(main_func_pid,cg::MEM);
    char buf[] = "fin";
    send(socks[0],buf,4,0);
    int *x;
    waitpid(main_func_pid,x,0);
    cgroup->getStatus();
    SUCCESS("sub func fin");
    if (kill(init_pid,SIGKILL) == 0 ){
      SUCCESS("init process killed");
    } else {
      ERROR("FAIL: KILL INIT PROCESS");
    }
    return 1;
}

int cgroupClean(){
    INFO("clean");
    cgroup->clean();
    return 1;
}
