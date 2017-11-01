/*
 * sign.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */
#include "sign.h"

using namespace std;

//client_fd to id互斥锁
pthread_mutex_t sig_mutex;

map <int, int> cids;

int sign_init(){
	pthread_mutex_init(&sig_mutex, NULL);
	return 0;
}

int sign_add(int client_fd, int id){
	pthread_mutex_lock(&sig_mutex);
	cids.insert(pair<int, int>(client_fd, id));
	log("sign_add cids.size:", cids.size());
	pthread_mutex_unlock(&sig_mutex);
	return 0;
}

int sign_getid(int client_fd){
	pthread_mutex_lock(&sig_mutex);
	map<int, int>::iterator it;
	it = cids.find(client_fd);
	if(it != cids.end()){
		int ret = it->second;
		pthread_mutex_unlock(&sig_mutex);
		return ret;
	}else{
		//log("sign_getid error. cids size:", cids.size());
		pthread_mutex_unlock(&sig_mutex);
		return -1;
	}
}

int sign_getfd(int id){
	pthread_mutex_lock(&sig_mutex);
	map<int, int>::iterator it;
	for(it = cids.begin(); it != cids.end(); it++){
		if(it->second == id){
			pthread_mutex_unlock(&sig_mutex);
			return it->first;
		}
	}
	pthread_mutex_unlock(&sig_mutex);
	return -1;
}

int sign_del(int client_fd){
	pthread_mutex_lock(&sig_mutex);
	if(cids.erase(client_fd) == 1){
		pthread_mutex_unlock(&sig_mutex);
		return 0;
	}else{
		pthread_mutex_unlock(&sig_mutex);
		log("sign_del error.");
		return -1;
	}
}

int sign_login_back(int client_fd, bool success, char* username){
	//s->c	title:21	num:1	data0:<char>username[max:20]
	//登录成功
	//s->c	title:29	num:0
	//登录失败
	char buff[BUFF_LEN];
	if(success){
		int title = 21;
		int num = 1;
		setHead(buff, title, num);
		char* (pdata)[num] = {username};
		int datalen[num] = {strlen(username)+1};
		int len = setData(buff, num, pdata, datalen);
		r_send(client_fd, buff, len);
	}else{
		int title = 29;
		int num = 0;
		setHead(buff, title, num);
		r_send(client_fd, buff, 8);
	}
	return 0;
}

int sign_login(int client_fd, int num, char* (pdata)[], int datalen[]){
	//c->s	title:20	num:2	data0:<int>userid	data1:<char>password[max:20,min:6]
	//发送登录请求
	int id = ByteArrayToInt(pdata[0]);
	int plen = datalen[1];
	char password[plen];
	ByteArrayToStr(pdata[1], password, plen);
	bool success = false;
	if(password[plen]!='\0'){
		log("sign_login error: \0 missed.");
	}
	char username[21];
	if(db_check_user(id, password) != 0){
		log("sign_login error: password incorrect.");
	}else{
		//登录成功
		success = true;
		onl_add(id);//向在线队列添加该用户ID
		sign_add(client_fd, id);//向鉴权队列添加用户ID
		if(db_get_username(id, username) != 0){
			log("sign_login=>db_get_username error.");
		}
	}
	sign_login_back(client_fd, success, username);
	if(success){
		return 0;
	}else{
		return -2;
	}
}

int sign_up_back(int client_fd, bool success, int id){
	//s->c	title:11	num:1	data0:<int>userid
	//注册成功，返回用户id
	//s->c	title:19	num:0
	//注册失败
	char buff[BUFF_LEN];
	if(success){
		int title = 11;
		int num = 1;
		setHead(buff, title, num);
		char c_id[4];
		IntToByteArray(id, c_id);
		char* (pdata)[num] = {c_id};
		int datalen[num] = {4};
		int len = setData(buff, num, pdata, datalen);
		r_send(client_fd, buff, len);
	}else{
		int title = 19;
		int num = 0;
		setHead(buff, title, num);
		r_send(client_fd, buff, 8);
	}
	return 0;
}

int sign_up(int client_fd, int num, char* (pdata)[], int datalen[]){
	//c->s	title:10	num:2	data0:<char>username[max:20]	data1:<char>password[max:20,min:6]
	//发送注册请求
	int ulen = datalen[0];
	char username[ulen];
	ByteArrayToStr(pdata[0], username, ulen);
	int plen = datalen[1];
	char password[plen];
	ByteArrayToStr(pdata[1], password, plen);
	if(username[ulen]!='\0' || password[plen]!='\0'){
		log("sign_up error: \0 missed.");
		return -1;
	}
	bool success = false;
	int id;
	if((id = db_insert_user(username, password)) >= 0){
		success = true;
	}
	sign_up_back(client_fd, success, id);

	return 0;
}

