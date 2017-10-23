#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "log.h"

#define PORT 9762


unsigned int len;
int n;

int init(){
	int server_fd , clint_fd;
	struct sockaddr_in servaddr, clinaddr;
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
