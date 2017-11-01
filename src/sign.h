/*
 * sign.h
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */

#ifndef SIGN_H_
#define SIGN_H_

#include <map>
#include <pthread.h>
#include <string.h>
#include "tools.h"
#include "router.h"
#include "online.h"

int sign_init();
int sign_add(int client_fd, int id);
int sign_getid(int client_fd);
int sign_getfd(int id);
int sign_del(int client_fd);

int sign_login(int client_fd, int num, char* (pdata)[], int datalen[]);
int sign_up(int client_fd, int num, char* (pdata)[], int datalen[]);

#endif /* SIGN_H_ */
