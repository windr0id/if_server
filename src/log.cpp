/*
 * log.cpp
 *
 *  Created on: Oct 22, 2017
 *      Author: windroid
 */

#include "log.h"
#include <iostream>

using namespace std;


void log(int a){
	cout<<a<<endl;
}

void log(const char* s){
	cout<<s<<endl;
}
void log(const char* s0, const char* s1){
	cout<<s0<<" "<<s1<<endl;
}
void log(const char* s, int code){
	cout<<s<<" "<<code<<endl;
}

void log(const char* s0,const char* s1, int code){
	cout<<s0<<" "<<s1<<" "<<code<<endl;
}
