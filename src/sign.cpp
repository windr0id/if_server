/*
 * sign.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */

#include "sign.h"


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
		return -1;
	}
	if(db_check_user(id, password) != 0){
		log("sign_login error: password incorrect.");
		return -1;
	}else{
		success = true;
	}
	char username[20];
	if(db_get_username(id, username) != 0){
		log("sign_login=>db_get_username error.");
	}
	sign_login_back(client_fd, success, username);

	return 0;
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

