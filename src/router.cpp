/*
 * router.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#include "router.h"

/**
 * 路由
 */
void r_route(int client_fd, int t, int num, char* (pdata)[], int datalen[]){
	if(t < 10){
		//nothing to do
	}else if(t <20){
		//用户注册
		sign_up(client_fd, t, num, pdata, datalen);
	}else if(t <30){
		//用户登录&&退出
	}else if(t <40){
		//消息
		mes_in(client_fd, t, num, pdata, datalen);
	}else if(t <50){
		//查询在线用户
	}else if(t <60){
		//文件传输
	}
}

/**
 * 接收报文
 */
int r_recv(int client_fd, char* buff){
	int n;
	while(true){
		if((n = recv(client_fd, buff, BUFF_LEN, 0)) < 0){
			if(errno == EAGAIN){
				//未收到报文
				return 0;
			}
			log("r_recv: socket error.", errno);
			pthread_exit(0);
		}else{
			return n;
		}
	}
}
/**
 * 发送报文
 */
int r_send(int client_fd, char* buff, int len){
    if(send(client_fd, buff, len, 0)<0){
        log("router=>r_send: send error.");
        return -1;
    }
    return 0;
}
/**
 * 报文解析
 */
int r_parse(char* buff, int* title, int* num, char* (pdata)[], int datalen[]){
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
    	int len = r_recv(client_fd, buff);
    	if(len == 0){
    		//没有收到新报文,检查是否有需要发送的消息
    		mes_out(client_fd, 976);
    	}else{
    		int title, num;
			char* (pdata)[MAX_DATA_NUM];
			int datalen[MAX_DATA_NUM];
			r_parse(buff, &title, &num, pdata, datalen);
			r_route(client_fd, title, num, pdata, datalen);

			log("----------------");
			log(title);
			log(num);
			log(datalen[0]);
			log(pdata[0]);
			log(datalen[1]);
			log(pdata[1]);
			log("-----router----");
    	}
    }
}



