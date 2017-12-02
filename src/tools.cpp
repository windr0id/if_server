/*
 * tools.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */
#include "head.h"

int threadDelay(const int sec, const int usec){
	timeval timeOut;
	timeOut.tv_sec = sec;
	timeOut.tv_usec = usec;
	if (0 != select(0, NULL, NULL, NULL, &timeOut))
	{
		return -1;
	}
	return 0;
}
/**
 * 字节流数据相关
 */
void IntToByteArray(int n, char* b) {
        b[0] =  (n & 0xff);
        b[1] =  (n >> 8 & 0xff);
        b[2] =  (n >> 16 & 0xff);
        b[3] =  (n >> 24 & 0xff);
        return;
}
int ByteArrayToInt(char* b) {
         return (int) ((((b[3] & 0xff) << 24)
                    | ((b[2] & 0xff) << 16)
                    | ((b[1] & 0xff) << 8)
                    | ((b[0] & 0xff) << 0)));
}
void ByteArrayToStr(char* b, char* dest, int len){
    for(int i=0; i<len; i++){
        dest[i] = b[i];
    }
    dest[len] = '\0';
}


void setHead(char* buff, int title, int num){
    char c_title[4];
    IntToByteArray(title, c_title);
    char c_num[4];
    IntToByteArray(num, c_num);

    memcpy((void*)(buff), (void*)c_title, 4);
    memcpy((void*)(buff+4), (void*)c_num, 4);
}

int setData(char* buff, int num, char* (pdata)[], int datalen[]){
    int length = 8;
    char* cursor = buff+8;
    char c_datalen[4];
    for(int i=0; i<num; i++){
        IntToByteArray(datalen[i], c_datalen);
        memcpy((void*)(cursor), (void*)c_datalen, 4);
        cursor += 4;
        memcpy((void*)(cursor), (void*)pdata[i], datalen[i]);
        cursor += datalen[i];
        length += 4+datalen[i];
    }
    return length;
}

/**
 * 报文解析
 */
int t_parse(char* buff, int* title, int* num, char* (pdata)[], int datalen[]){
	*title = ByteArrayToInt(buff);
	*num = ByteArrayToInt(buff+4);
	char* cursor = buff+8;
	for(int i=0; i<*num; i++){
		datalen[i] = ByteArrayToInt(cursor);
		pdata[i] = cursor+4;
		cursor += 4+datalen[i];
	}
	return 0;
}
