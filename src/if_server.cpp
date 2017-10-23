#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "log.h"

#define PORT 9762

char buff[4096];

void c_rev(int* arg){
	int clint_fd = *arg;
	int n;
    while(true){
        if((n = recv(clint_fd, buff, 20, 0)) < 0){
            sleep(100);
        }else{
            buff[n] = '\0';
            if(strcmp(buff, "over") == 0){
                log("child over");
                pthread_exit(0);
            }
            log(buff, "info from:", clint_fd);
        }
    }

}

int main(){
    int server_fd , clint_fd;
    struct sockaddr_in servaddr, clinaddr;
    unsigned int len;

    if((server_fd = socket(AF_INET , SOCK_STREAM, 0)) < 0){
        printf("create socket error\n");
        exit(0);
    }
   //printf("%d\n", server_fd);

    int opt = 1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if(bind(server_fd , (struct sockaddr*)&servaddr, sizeof(servaddr)) <0 ){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
    }

    if(listen(server_fd, 5) <0){
        log("listen socket error.");
        exit(0);
    }

    while(true){
        if((clint_fd = accept(server_fd, (struct sockaddr*)&clinaddr, &len)) <0){
            log("accept socket error.");
            exit(0);
        }

        char ans[3] = "ok";
        if(send(clint_fd, ans, strlen(ans), 0) <0){
            log("send error.");
            exit(0);
        }
        pthread_t pid;
        int ret = pthread_create(&pid, NULL, (void * (*)(void *))c_rev, (void *)&clint_fd);
        log(ret);
        if(ret != 0){
        	log("pthread create error.");
        }else{
        	log("pthread create success.", pid);
        }
    }

    return 0;
}
