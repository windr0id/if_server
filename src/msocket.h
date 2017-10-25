/*
 * info.h
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#ifndef MSOCKET_H_
#define MSOCKET_H_

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


#endif /* MSOCKET_H_ */
