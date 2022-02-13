#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
int main(){
    for (int i=0;i<=114514;i++){
        printf("%d:",i);
        printf("number of the signal that caused the child process to terminate : %d\n",WTERMSIG(i));
        printf("%d\n",WSTOPSIG(i));
       // printf("the number of the signal which caused the child to stopped:%d\n",WSTOPSIG(wstatus));
    }
}