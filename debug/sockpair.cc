#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
int main(){
    int fd[2];
    socketpair(AF_UNIX,SOCK_SEQPACKET,0,fd);
    char buf[]="hey";
    send(fd[0],buf,sizeof(buf),0);
    char buff[100];
    recv(fd[1],buff,4,0);
    printf("%s",buff);
}