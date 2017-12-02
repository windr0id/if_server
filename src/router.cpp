/*
 * router.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#include "head.h"

/**
 * 路由
 */
void r_route(int client_fd, int t, int num, char* (pdata)[], int datalen[]){
	switch(t){
	case 10:
		//用户注册
		sign_up(client_fd, num, pdata, datalen);
		break;
	case 20:
		//用户登录&&退出
		sign_login(client_fd, num, pdata, datalen);
		break;
	case 30:
		//消息
		mes_in(client_fd, num, pdata, datalen);
		break;
	case 40:
		//查询在线用户
		onl_query(client_fd);
		break;
	case 50:
		//传输请求
		p2p_in(client_fd, num, pdata, datalen);
		break;
	}
}

/**
 * 接收报文
 */
int r_recv(int client_fd, char* buff){
	int n;
	while(true){
		if((n = recv(client_fd, buff, BUFF_LEN, 0)) < 0){
			switch(errno){
			case EAGAIN:
				//稍后重试
				//log("EAGAIN");
				return 0;
			}
			log("r_recv: socket error.", errno);
			pthread_exit(0);
		}else if(n == 0){
			//连接断开
			log("client_fd close: ", client_fd);
			onl_del(sign_getid(client_fd));//从在线队列删除
			sign_del(client_fd);//从鉴权队列删除
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
        log("router=>r_send: send error. client_fd: ", client_fd);
        return -1;
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
    		mes_out(client_fd);
    		p2p_out(client_fd);
    	}else{
    		int title, num;
			char* (pdata)[MAX_DATA_NUM];
			int datalen[MAX_DATA_NUM];
			t_parse(buff, &title, &num, pdata, datalen);
			r_route(client_fd, title, num, pdata, datalen);
    	}
    	//延迟100ms
    	threadDelay(0, 100000);
    }
}



