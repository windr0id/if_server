/*
 * info.h
 *
 *  Created on: Oct 23, 2017
 *      Author: windroid
 */
#ifndef MSOCKET_H_
#define MSOCKET_H_

#include "head.h"

int socket_init();
void socket_wait();
int socket_err(int fd);

#endif /* MSOCKET_H_ */
