/*
 * info.h
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#ifndef INFO_H_
#define INFO_H_

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
#include "router.h"

#define PORT 9762

int socket_init();
void socket_wait();


#endif /* INFO_H_ */
