#include <cstring>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <iostream>
void init_core_dump()
{
    struct rlimit limit;
    memset(&limit, 0, sizeof(limit));
    limit.rlim_cur = 1; 
    limit.rlim_max = 1;
    setrlimit(RLIMIT_CPU, &limit);
}
const int len = 4096 * 12;

int main_func(void *p){
    int q;
    while (1){
        std::cout<<"hi"<<std::endl;
    }
    std::cout<<"HI"<<std::endl;
}
int main(void)
{
    
    struct rlimit oldlimit;
    struct rlimit limit;
    memset(&limit, 0, sizeof(limit));
    limit.rlim_cur = 1; 
    limit.rlim_max = 1;
    void *stack = mmap(NULL,len,PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_ANONYMOUS | MAP_SHARED,-1,0);
    
    int main_func_pid =  clone(main_func,stack+len,SIGCHLD,NULL);
    
    prlimit(main_func_pid,RLIMIT_CPU,&limit,&oldlimit);
    int stat_loc;
    waitpid(main_func_pid,&stat_loc,NULL);
    int q = WTERMSIG(stat_loc);
    printf("%d",q);
    return 0;
}