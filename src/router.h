/*
 * router.h
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "message.h"
#include "sign.h"
#include "tools.h"

#define BUFF_LEN	4096
#define MAX_DATA_NUM 24

void r_thread(int*);
int r_send(int client_fd, char* buff, int len);

#endif /* ROUTER_H_ */
