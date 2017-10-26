/*
 * sign.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */

#include "sign.h"

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
		//todo
		break;
	default:
	}
	return 0;
}

