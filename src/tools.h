/*
 * tools.h
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <iostream>
#include <string.h>

void IntToByteArray(int n, char* b);
int ByteArrayToInt(char* b);
void ByteArrayToStr(char* b, char* dest, int len);

void setHead(char* buff, int title, int num);
int setData(char* buff, int num, char* (pdata)[], int datalen[]);


#endif /* TOOLS_H_ */
