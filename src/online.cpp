/*
 * online.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: windroid
 */

#include "online.h"

using namespace std;

//在线用户互斥锁
pthread_mutex_t onl_mutex;

set <int> onluser;

int onl_init(){
	onl_add(10000);
	pthread_mutex_init(&onl_mutex, NULL);
	return 0;
}

bool isOnline(int id){
	set<int>::iterator it;
	pthread_mutex_lock(&onl_mutex);
	it = onluser.find(id);
	if(it != onluser.end()){
		pthread_mutex_unlock(&onl_mutex);
		return true;
	}else{
		pthread_mutex_unlock(&onl_mutex);
		return false;
	}
}

void onl_add(int id){
	pthread_mutex_lock(&onl_mutex);
	onluser.insert(id);
	pthread_mutex_unlock(&onl_mutex);
}

void onl_del(int id){
	pthread_mutex_lock(&onl_mutex);
	onluser.erase(id);
	pthread_mutex_unlock(&onl_mutex);
}

int onl_gets(int out_onlarr[]){
	set<int>::iterator it = onluser.begin();
	int i = 0;
	pthread_mutex_lock(&onl_mutex);
	for(; it != onluser.end(); it++, i++){
		out_onlarr[i] = *it;
	}
	pthread_mutex_unlock(&onl_mutex);
	return i;
}

int onl_query(int client_fd){
	//c->s	title:40	num:0
	//发送查询请求
	//s->c	title:41	num:2x	data0:<int>userid0	data1:<char>username0[] ...
	//返回在线用户的id和用户名
	//s->c	title:49	num:0
	//查询失败
	char buff[BUFF_LEN];
	int onlarr[500];
	int n = onl_gets(onlarr);
	int title = 41;
	int num = n*2;
	setHead(buff, title, num);
	char* (pdata)[num];
	int datalen[num];
	for(int i=0; i<n; i++){
		char* c_id = new char[4];
		IntToByteArray(onlarr[i], c_id);
		char* username = new char[21];
		db_get_username(onlarr[i], username);
		pdata[i*2] = c_id;
		pdata[i*2+1] = username;
		datalen[i*2] = 4;
		datalen[i*2+1] = strlen(username)+1;
	}
	int len = setData(buff, num, pdata, datalen);
	r_send(client_fd, buff, len);
	return 0;
}
