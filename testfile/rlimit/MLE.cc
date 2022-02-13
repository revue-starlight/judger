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
const int len = 4096 * 12;
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
 
int get_memory_by_pid(__pid_t p){
    FILE *fd;
    char name[32], line_buff[256] = {0}, file[64] = {0};
    int i, vmrss = 0;
 
    sprintf(file, "/proc/%d/status", p);
    fd = fopen(file, "r");
    if(fd==NULL){
        fprintf(stderr,"open %s fail\n",file);
        exit(1);
    }
 
    // 读取VmSize这一行的数据
    for (i = 0; i < 40; i++){
        if (fgets(line_buff, sizeof(line_buff), fd) == NULL){
            break;
        }
        if (strstr(line_buff, "VmSize") != NULL){
            sscanf(line_buff, "%s %d", name, &vmrss);
            break;
        }
    }
 
    fclose(fd);
    return vmrss;
}

int main_func(void *p){
  sleep(20);
    while(1){
        unsigned long virt_mem_sz = get_memory_by_pid(getpid());
        printf("now mem used : 0x%lx kB\n",virt_mem_sz);
        void *str = malloc(1024*1024);
    }
    return 0;
}

int main(void)
{
    
    struct rlimit oldlimit;
    struct rlimit limit;
    memset(&limit, 0, sizeof(limit));
    limit.rlim_cur = 1024*1024*64; 
    limit.rlim_max = 1024*1024*128;
    void *stack = mmap(NULL,len,PROT_READ | PROT_WRITE | PROT_EXEC,
    MAP_ANONYMOUS | MAP_SHARED,-1,0);
    
    int main_func_pid =  clone(main_func,(void*)((size_t)stack+len),SIGCHLD,NULL); 
    std::cout<<main_func_pid<<std::endl;
    int stat_loc;
    waitpid(main_func_pid,&stat_loc,0);
    int q = WTERMSIG(stat_loc);
    printf("%d\n",q);
    return 0;
}
