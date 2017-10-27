/*
 * sign.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */

#include "sign.h"

int id_dex = 10000;
struct idpool{
	char* username;
	int ulen;
	int id;
}idp;

int getId(char* username, int ulen){

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
		return 0;
	}else{
		int title = 19;
		int num = 0;
		setHead(buff, title, num);
		r_send(client_fd, buff, 8);
		return 0;
	}

	return 0;
}

int sign_up(int client_fd, int t, int num, char* (pdata)[], int datalen[]){
	switch(t)
	{
	case 10:
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
		if((id = db_sign_up(username, password)) >= 0){
			success = true;
		}
		sign_up_back(client_fd, success, id);
		break;
	}
	return 0;
}

