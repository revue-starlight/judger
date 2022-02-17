#include <cstring>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <iostream>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
const int len = 4096 * 12;
int main_func(void *arg){
    errno = 0;
    const char* fileName = "/root/repos/judger/output/2.out";
    FILE *fileP;
    fileP = fopen(fileName,"r");
    if (fileP == NULL)
    {
        fileP = fopen(fileName, "w");
    }
    fclose(fileP);
    int fd = open(fileName, O_WRONLY, 0666);
    dup2(fd, STDOUT_FILENO); // Check `man stdin` for more info
    execl("/root/repos/judger/testfile/sort6.out","/root/repos/judger/testfile/sort6.out",NULL);
}

int main(){
    void *stack = mmap(NULL,len,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_ANONYMOUS | MAP_SHARED,-1,0);
    rlimit fsizeRes;
    fsizeRes.rlim_cur = 2400;
    fsizeRes.rlim_max = 3200;
    //setrlimit(RLIMIT_FSIZE,&fsizeRes);
    int main_func_pid =  clone(main_func,(void*)((size_t)stack+len),SIGCHLD,NULL); 
    prlimit(main_func_pid,RLIMIT_FSIZE,&fsizeRes,NULL);
    std::cout<<main_func_pid<<std::endl;
    int stat_loc;
    waitpid(main_func_pid,&stat_loc,0);
    printf("%d\n",WIFSIGNALED(stat_loc));
    printf("%d\n",WTERMSIG(stat_loc));
    printf("%d\n",WSTOPSIG(stat_loc));
}