#include "head.h"

int server_fd;
struct sockaddr_in servaddr, clinaddr;

/**
 * 初始化操作
 * 创建socket 绑定端口 监听端口
 */
int socket_init(){
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
 * 主线程在此阻塞 等待客户端请求
 */
void socket_wait(){
	int client_fd;
	unsigned int len;
    while(true){
    	len = sizeof((struct sockaddr *) &clinaddr);
        if((client_fd = accept(server_fd, (struct sockaddr*)&clinaddr, &len)) <0){
            log("accept socket error.", errno);
            log(server_fd);
            exit(0);
        }
        log("accept a client. client_fd:", client_fd);
        //设置client_fd为非阻塞模式
        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

        pthread_t pid;
        int ret = pthread_create(&pid, NULL, (void * (*)(void *))r_thread, (void *)&client_fd);
        if(ret != 0){
        	log("pthread create error.");
        }else{
        	log("pthread create success. pid:", pid);
        }
    }
}

int socket_err(int fd){
	int status;
	socklen_t slen;
	getsockopt(fd, SOL_SOCKET, SO_ERROR, (void *) &status, &slen);
	return status;
}
