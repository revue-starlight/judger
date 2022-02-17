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
#include <unistd.h>
#include <assert.h>
#include <string>
#include <sys/types.h>
#include "../src/utils/allutils.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
const int len = 4096 * 12;
int main_func(void *arg){

}

int main(){
    errno = 0;
    // void *stack = mmap(NULL,len,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_ANONYMOUS | MAP_SHARED,-1,0);
    // int main_func_pid =  clone(main_func,(void*)((size_t)stack+len),SIGCHLD,NULL); 
    // std::cout<<main_func_pid<<std::endl;
    // int stat_loc;
    // waitpid(main_func_pid,&stat_loc,0);
    errno = 0;
    const char* fileName = "1.in";
    FILE *fileP;
    fileP = fopen(fileName,"r");
    if (fileP == NULL)
    {
        fileP = fopen(fileName, "w");
    }
    fclose(fileP);
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC);
    dup2(fd, STDIN_FILENO); // Check `man stdin` for more info
    close(fd);


    // freopen(fileName,"r",stdin);
    // execl("/root/repos/judger/testfile/sort6.out","/root/repos/judger/testfile/sort6.out",NULL);
    printFD(getpid());
    printf("%s\n",strerror(errno));
    while (1){
        char ch;
        scanf("%c",&ch);
        printf("%c",ch);
    }
}