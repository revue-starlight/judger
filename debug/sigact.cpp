#include "../src/utils/log.hpp"
#include <bits/stdc++.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
using namespace std;
void handle(int signum,siginfo_t *info,void *ucontext){
    INFO("wow u want signum %d",signum);
    cout<<info->si_code<<"\n";
    cout<<info->si_errno<<"\n";
    cout<<info->si_signo<<"\n";
    ucontext_t *con = (ucontext_t*)ucontext;
    cout<<con->uc_flags<<endl;
    INFO("END");
}
int main(){
    struct sigaction s;
    siginfo_t siginfo;

    s.sa_flags = SA_SIGINFO;
    s.sa_sigaction = handle;
    unsigned long long p;
    sigaction(SIGINT,&s,NULL);
    while(1){
        p++;
        if (p%1000000000==0) {INFO("%d",p); p=0;}
    }
    
    return 0;
}