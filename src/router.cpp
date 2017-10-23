/*
 * router.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#include "router.h"

//路由
void route(int t){
	if(t < 10){
		//nothing to do
	}else if(t <20){
		//用户注册
	}else if(t <30){
		//用户登录&&退出
	}else if(t <40){
		//消息
	}else if(t <50){
		//查询在线用户
	}else if(t <60){
		//文件传输
	}
}
/**
 * int32与bytes相互转化
 */
void IntToByteArray(int n, char* b) {
        b[0] =  (n & 0xff);
        b[1] =  (n >> 8 & 0xff);
        b[2] =  (n >> 16 & 0xff);
        b[3] =  (n >> 24 & 0xff);
        return;
}
int ByteArrayToInt(char* b) {
         return (int) ((((b[3] & 0xff) << 24)
                    | ((b[2] & 0xff) << 16)
                    | ((b[1] & 0xff) << 8)
					| ((b[0] & 0xff) << 0)));
}
int getData(){
	return 0;
}
/**
 * recv不保证一次接收完指定的size
 * 封装为mrecv保证接收完指定的size
 */
int mrecv(int client_fd, char* buff, int size){
	int n, len = 0;
	while(true){
		if(len >= size){
			return 0;
		}
		if((n = recv(client_fd, buff+len, 4096, 0)) < 0){
			//sleep(10);
		}else{
			len += n;
			log(len);
		}
	}

	return 0;
}
/**
 * 分配给每个客户端的子线程
 */
void r_thread(int* arg){
	char buff[4096];
	memset(&buff, 9, sizeof(buff));

	int client_fd = *arg;
	mhead m;
    while(true){
		mrecv(client_fd, buff, HEADLEN);
		for(int i=0; i<8; i++){
				printf("%d-", buff[i]);
			}
		char* pb = buff;
		int title = ByteArrayToInt(pb);
		int num = ByteArrayToInt(pb+4);
		log("Strut mhead:");
		log(title);
		log(num);


    }
}



