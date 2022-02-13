#include <iostream>
#include <sys/mman.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int init_fn(void *arg){
    int a[3];
    a[114514]=8;
    return 0;

}

int LEN = 1024*128;
int main(){
    void* stack = mmap(NULL,LEN,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,0,NULL);
    clone(init_fn,(void*)((size_t)stack+LEN),SIGCHLD,NULL);
    int status;
    wait(&status);
    if (WIFSIGNALED(status) == true){
        cout<<WTERMSIG(status)<<endl;
    }
    return 0;
}
