/*
 * message.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */
#include "message.h"


//消息队列互斥锁
pthread_mutex_t mes_mutex;

//消息队列
struct message{
	bool active;
	int tag;
	int sourceid;
	int destid;
	char message[4000];
	int meslen;
}mesque[MESSAGE_QUE_LEN];

void mes_init(){
	pthread_mutex_init(&mes_mutex, NULL);
	for(int i=0; i<MESSAGE_QUE_LEN; i++){
		mesque[i].active = false;
	}
}

int addMes(int tag, int sourceid, int destid, char* message, int meslen){
	pthread_mutex_lock(&mes_mutex);
	for(int i=0; i<MESSAGE_QUE_LEN; i++){
		if(mesque[i].active == false){
			mesque[i].active = true;
			mesque[i].tag = tag;
			mesque[i].sourceid = sourceid;
			mesque[i].destid = destid;
			memcpy(mesque[i].message, message, meslen);
			mesque[i].meslen = meslen;
			pthread_mutex_unlock(&mes_mutex);
			log("addMes:", message);
			return 0;
		}else{
			continue;
		}
	}
	pthread_mutex_unlock(&mes_mutex);
	return -1;
}

int getMesIndex(int destid){
	pthread_mutex_lock(&mes_mutex);
	for(int i=0; i<MESSAGE_QUE_LEN; i++){
		if(mesque[i].active == false){
			continue;
		}
		if(mesque[i].destid == destid){
			pthread_mutex_unlock(&mes_mutex);
			return i;
		}
	}
	pthread_mutex_unlock(&mes_mutex);
	return -1;
}

void delMes(int index){
	pthread_mutex_lock(&mes_mutex);
	mesque[index].active = false;
	pthread_mutex_unlock(&mes_mutex);
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
int mes_back(int client_fd, int tag, int sourceid, char* mes, int meslen){
	//s->c	title:33	num:3	data0:<int>-tag	data1:<int>sourceid	data2:<char>message[max:4000]
	//分发标识为-tag的消息
	char buff[BUFF_LEN];
	int title = 33;
	int num = 3;
	setHead(buff, title, num);

	int datalen[num] = {4, 4, meslen};
	char c_tag[4];
	IntToByteArray(-tag, c_tag);
	char c_sourceid[4];
	IntToByteArray(sourceid, c_sourceid);
	char message[meslen];
	memcpy(message, mes, meslen);
	char* (pdata)[num] = {c_tag, c_sourceid, message};
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

int mes_out(int client_fd, int id){
	int index;
	if((index = getMesIndex(id)) < 0){
		//没有消息
		//log("mes_out: no mes");
		return 0;
	}else{
		message* m = &mesque[index];
		log("mes_out: ", m->tag);
		mes_back(client_fd, m->tag, m->sourceid, m->message, m->meslen);
		delMes(index);
	}
	return 0;
}
