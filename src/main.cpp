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
#include <sys/resource.h>
#include "utils/allutils.hpp"
#include "utils/log.hpp"
#include "cgroup.hpp"
#include "spawn.hpp"
#include "config.hpp"
#include "result.hpp"
using namespace std;
namespace fs = filesystem;
int setCgroup();
int execute();
int cgroupClean();
cg::Cgroup *cgroup;
Config *config;
Result *result;
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
    sp.pivot_root(config);
    INFO("mount proc");
    mount("proc","/proc","proc",0,NULL);

    INFO("opening file");
   
    string fileName = config->getOutputName();
    string inputName = config->getInputName();
    WARNING("inputname = %s",inputName.c_str());
    WARNING("command = %s",config->getCommand().c_str());
    


    FILE *fileP;
    // fileP = fopen(fileName.c_str(),"r");
    // if (fileP == NULL)
    // {
    //     fileP = fopen(fileName.c_str(), "w");
    // }
    // fclose(fileP);
    // int inputFD = open(inputName.c_str(), O_WRONLY, 0666);
    // dup2(inputFD, STDIN_FILENO);


    fileP = fopen(fileName.c_str(),"r");
    if (fileP == NULL)
    {
        fileP = fopen(fileName.c_str(), "w");
    }
    fclose(fileP);
    int outputFD = open(fileName.c_str(), O_WRONLY, 0666);
    SUCCESS("redirect filename to %s",fileName.c_str());
    dup2(outputFD, STDOUT_FILENO); // Check `man stdin` for more info

    char buf[4];
    unshare(CLONE_NEWUSER);
    setuid(config->getUid());
    setgid(config->getGid());
    recv(*fd,buf,4,0);
    execl("/bin/bash","/bin/bash",NULL);
    //execl(config->getCommand().c_str(),config->getCommand().c_str(),NULL);
    return 0;
}

int execute(){
    cgroup->createAll();
    cgroup->write(cg::MEM);
    cgroup->write(cg::CPU);
    int socks[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,socks);
    int stackSize = 512*sysconf(_SC_PAGE_SIZE);
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

    int main_func_pid = clone(clone_main_func,(void*)((char*)alloca(stackSize)+stackSize),//CLONE_NEWUSER |//  CLONE_NEWIPC | CLONE_NEWUTS |
      CLONE_NEWNS | SIGCHLD,socks+1);
    cgroup->bind(main_func_pid,cg::CPU);
    cgroup->bind(main_func_pid,cg::MEM);


    rlimit CPURlimit = config->getTimeRlimit();
    rlimit MEMRlimit = config->getMemRlimit();
    prlimit(main_func_pid,RLIMIT_CPU,&CPURlimit,NULL);
    prlimit(main_func_pid,RLIMIT_AS,&MEMRlimit,NULL);

    char buf[] = "fin"; send(socks[0],buf,4,0);
    result = new Result;
    int stat_loc;
    waitpid(main_func_pid,&stat_loc,0);
    result->returnValue = stat_loc;
    INFO("process return %d",result->returnValue);
    cgroup->getStatus(*result);
    result->checkValid(config); 
    result->printResult();
    SUCCESS("task to run in sandbox complete");

    if (kill(init_pid,SIGKILL)==0){
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
