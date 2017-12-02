/*
 * message.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */
#include "head.h"

using namespace std;

//消息队列互斥锁
pthread_mutex_t mes_mutex;

//消息队列
struct mes_struct{
	int tag;
	int sourceid;
	int destid;
	char message[4000];
	int meslen;
	friend bool operator < (struct mes_struct const &a,struct mes_struct const &b){
		return a.destid < b.destid;
	}
};
set<mes_struct> messet;

void mes_init(){
	pthread_mutex_init(&mes_mutex, NULL);
}

int addMes(int tag, int sourceid, int destid, char* message, int meslen){
	pthread_mutex_lock(&mes_mutex);
	mes_struct m;
	m.tag = tag;
	m.sourceid = sourceid;
	m.destid = destid;
	memcpy(m.message, message, meslen);
	m.meslen = meslen;
	messet.insert(m);
	log("addMes:", message);
	pthread_mutex_unlock(&mes_mutex);
	return 0;
}

/**
 * 收到消息
 */
int mes_in(int client_fd, int num, char* (pdata)[], int datalen[]){
	//c->s	title:30	num:3	data0:<int>tag	data1:<int>desid[8]	data2:<char>message[max:4000]
	int tag = ByteArrayToInt(pdata[0]);
	int destid = ByteArrayToInt(pdata[1]);
	int meslen = datalen[2]+1;
	char mes[meslen];
	memcpy(mes, pdata[2], meslen);
	mes[meslen-1] = '\0';
	addMes(tag, sign_getid(client_fd), destid, mes, meslen);//添加消息至队列

	log("message in: ");
	log("tag: ", tag);
	log("destid: ", destid);
	log("message: ", mes);
	log("meslen: ", meslen);

	return 0;
}


/**
 * 分发消息
 */
int mes_back(int client_fd, int tag, int sourceid, const char* mes, int meslen){
	//s->c	title:33	num:3	data0:<int>-tag	data1:<int>sourceid	data2:<char>message[max:4000]
	//分发标识为-tag的消息
	char buff[BUFF_LEN];
	int title = 33;
	int num = 3;
	setHead(buff, title, num);

	int datalen[MAX_DATA_NUM] = {4, 4, meslen};
	char c_tag[4];
	IntToByteArray(-tag, c_tag);
	char c_sourceid[4];
	IntToByteArray(sourceid, c_sourceid);
	char message[meslen];
	memcpy(message, mes, meslen);
	char* (pdata)[MAX_DATA_NUM] = {c_tag, c_sourceid, message};
	int len = setData(buff, num, pdata, datalen);
	log("--------------------");
	log("tag: ", tag);
	log("sourceid: ", sourceid);
	log("mes: ", mes);
	log("meslen: ", meslen);
	log("------mes_back------");
	r_send(client_fd, buff, len);
	return 0;
}

int mes_out(int client_fd){
	int id = sign_getid(client_fd);
	pthread_mutex_lock(&mes_mutex);
	set<mes_struct>::iterator it = messet.begin();
	for(; it != messet.end(); it++){
		if(it->destid == id){
			mes_back(client_fd, it->tag, it->sourceid, (it->message), it->meslen);
			messet.erase(it);
		}
	}
	pthread_mutex_unlock(&mes_mutex);

	return 0;
}
