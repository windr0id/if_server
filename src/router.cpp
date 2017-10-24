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
/**
 * 线程在此阻塞
 * 接收一条报文
 */
int m_recv(int client_fd, char* buff){
	int n;
	while(true){
		if((n = recv(client_fd, buff, BUFF_LEN, 0)) <= 0){
			if(errno == EINTR){
				log("socket error: EINTER");
			}else{
				log("close socket: ", client_fd);
				close(client_fd);
				pthread_exit(0);
			}
			sleep(10);
		}else{
			return n;
		}
	}
}
/**
 * 报文解析
 */
int m_parse(char* buff, int* title, int* num, char* (pdata)[], int datalen[]){
	*title = ByteArrayToInt(buff);
	*num = ByteArrayToInt(buff+4);
	char* cursor = buff+8;
	for(int i=0; i<*num; i++){
		datalen[i] = ByteArrayToInt(cursor);
		pdata[i] = cursor+4;
		cursor += 4+datalen[i];
	}
	return 0;
}
/**
 * 分配给每个客户端的子线程
 */
void r_thread(int* arg){
	char buff[BUFF_LEN];
	memset(&buff, 99, sizeof(buff));

	int client_fd = *arg;
    while(true){
    	int len;//收到的报文长度
    	len = m_recv(client_fd, buff);

		int title, num;
		char* (pdata)[MAX_DATA_NUM];
		int datalen[MAX_DATA_NUM];
		m_parse(buff, &title, &num, pdata, datalen);
		log(title);
		log(num);
		log(datalen[0]);
		log(ByteArrayToInt(pdata[0]));
		log(datalen[1]);
		log(ByteArrayToInt(pdata[1]));
    }
}



