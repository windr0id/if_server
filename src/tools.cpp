/*
 * tools.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */

#include "tools.h"

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
