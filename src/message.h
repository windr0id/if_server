/*
 * message.h
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "head.h"

void mes_init();
int mes_in(int client_fd, int num, char* (pdata)[], int datalen[]);
int mes_out(int cliend_fd);

#endif /* MESSAGE_H_ */
