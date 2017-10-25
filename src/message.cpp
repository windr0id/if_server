/*
 * message.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */
#include "message.h"


int m_do(int client_fd, int t, int num, char* (pdata)[], int datalen[]){
	switch(t)
	{
	case 30:
		//c->s	title:30	num:3	data0:<int>tag	data1:<int>desid[8]	data2:<char>message[max:4000]
		int tag = ByteArrayToInt(pdata[0]);
		int destid = ByteArrayToInt(pdata[1]);
		char mes[datalen[2]+1];
		ByteArrayToStr(pdata[2], mes, datalen[2]);
		log("message in: ");
		log("tag: ", tag);
		log("destid: ", destid);
		log("message: ", mes);
		break;

	}
	return 0;
}


