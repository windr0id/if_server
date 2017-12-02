/*
 * router.h
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include "head.h"


void r_thread(int*);
int r_send(int client_fd, char* buff, int len);

#endif /* ROUTER_H_ */
