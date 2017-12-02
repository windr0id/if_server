/*
 * p2p.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: windroid
 */
#include "head.h"

using namespace std;

//p2p请求队列互斥锁
pthread_mutex_t p2p_req_mutex;
//p2p链接队列互斥锁
pthread_mutex_t p2p_link_mutex;

struct p2p_req_struct{
	int tag;
	int sourceid;
	int destid;
	int filesize;
	char filename[256];
	int fnamelen;
	friend bool operator < (struct p2p_req_struct const &a,struct p2p_req_struct const &b){
		return a.destid < b.destid;
	}
};

struct p2p_link_struct{
	int tag;
	int sourceid;
	int destid;
	int filesize;
	char filename[256];
	int fnamelen;
	friend bool operator < (struct p2p_link_struct const &a,struct p2p_link_struct const &b){
		return a.destid < b.destid;
	}
};

set<p2p_req_struct> p2p_req_set;

void p2p_init(){
	pthread_mutex_init(&p2p_req_mutex, NULL);
}

int addp2p(int tag, int sourceid, int destid, int filesize, char* filename, int fnamelen){
	pthread_mutex_lock(&p2p_req_mutex);
	p2p_req_struct m;
	m.tag = tag;
	m.sourceid = sourceid;
	m.destid = destid;
	m.filesize = filesize;
	memcpy(m.filename, filename, fnamelen);
	m.fnamelen = fnamelen;
	p2p_req_set.insert(m);
	log("add p2p:", filename);
	pthread_mutex_unlock(&p2p_req_mutex);
	return 0;
}

/**
 * 收到p2p请求
 */

void p2p_in(int client_fd, int num, char* (pdata)[], int datalen[]){
	//c->s	title:50	num:4	data0:<int>tag	data1:<int>destid	data2:<int>filesize	data3:<char>filename[max:256]
	//发起传输请求
	int tag = ByteArrayToInt(pdata[0]);
	int destid = ByteArrayToInt(pdata[1]);
	int fsize = ByteArrayToInt(pdata[2]);
	int fnamelen = datalen[3]+1;
	char fname[fnamelen];
	memcpy(fname, pdata[3], fnamelen);
	fname[fnamelen-1] = '\0';

	addp2p(tag, sign_getid(client_fd), destid, fsize, fname, fnamelen);
}

/**
 * 分发p2p请求
 */

void p2p_req_back(int client_fd, int tag, int sourceid, int filesize, const char* filename, int fnamelen){
	//s->c	title:55	num:4	data0:<int>-tag	data1:<int>sourceid	data2:<int>filesize	data3:<char>filename[max:256]
	//分发传输请求
	char buff[BUFF_LEN];
	int title = 55;
	int num = 4;
	setHead(buff, title, num);

	int datalen[MAX_DATA_NUM] = {4, 4, 4, 4, fnamelen};
	char c_tag[4];
	IntToByteArray(-tag, c_tag);
	char c_sourceid[4];
	IntToByteArray(sourceid, c_sourceid);
	char c_fsize[4];
	IntToByteArray(filesize, c_fsize);
	char fname[fnamelen];
	memcpy(fname, filename, fnamelen);
	char* (pdata)[MAX_DATA_NUM] = {c_tag, c_sourceid, c_fsize, fname};
	int len = setData(buff, num, pdata, datalen);

	r_send(client_fd, buff, len);
}

void p2p_link_back(){


}

int p2p_out(int client_fd){
	int id = sign_getid(client_fd);
	pthread_mutex_lock(&p2p_req_mutex);
	set<p2p_req_struct>::iterator it = p2p_req_set.begin();
	for(; it != p2p_req_set.end(); it++){
		if(it->destid == id){
			p2p_req_back(client_fd, it->tag, it->sourceid,it->filesize, (it->filename), it->fnamelen);
			p2p_req_set.erase(it);
		}
	}
	pthread_mutex_unlock(&p2p_req_mutex);
	return 0;
}



