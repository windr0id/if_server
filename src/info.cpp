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

int server_fd;
struct sockaddr_in servaddr, clinaddr;

bool accepting = true;

/**
 * 初始化操作
 * 创建socket 绑定端口 监听端口
 */
int init(){
	//创建socket
    if((server_fd = socket(AF_INET , SOCK_STREAM, 0)) < 0){
        log("create socket error.");
        return -1;
    }
    //设置端口立即可用
    int opt = 1;
    setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //初始化servaddr struct
    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	//绑定端口
    if(bind(server_fd , (struct sockaddr*)&servaddr, sizeof(servaddr)) <0 ){
        log("bind socket error. ", strerror(errno), errno);
        return -2;
    }
    //监听队列设为5
    if(listen(server_fd, 5) <0){
	   log("listen socket error.");
	   return -3;
    }
    return 0;
}

/**
 * 分配给每个客户端的子线程
 */
void c_thread(int* arg){
	char buff[4096];
	int clint_fd = *arg;
	int n;
    while(true){
        if((n = recv(clint_fd, buff, 20, 0)) < 0){
            //sleep(100);
        }else{
        	//debug用 除去ctrl+c
        	if(buff[0] <= 26){
        		pthread_exit(0);
			}
            buff[n] = '\0';
            if(strcmp(buff, "over") == 0){
                log("child over");
                pthread_exit(0);
            }
            log(buff, "<from>:", clint_fd);
        }
    }
}

/**
 * 主线程在此阻塞 等待客户端请求
 */
void wait(){
	int client_fd;
	unsigned int len;
    while(accepting){
        if((client_fd = accept(server_fd, (struct sockaddr*)&clinaddr, &len)) <0){
            log("accept socket error.", errno);
            log(server_fd);
            exit(0);
        }
        log("accept a client", client_fd);
        pthread_t pid;
        int ret = pthread_create(&pid, NULL, (void * (*)(void *))c_thread, (void *)&client_fd);
        if(ret != 0){
        	log("pthread create error.");
        }else{
        	log("pthread create success.", pid);
        }
    }

}
