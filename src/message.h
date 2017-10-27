/*
 * message.h
 *
 *  Created on: Oct 25, 2017
 *      Author: windroid
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string.h>
#include <pthread.h>

#include "log.h"
#include "tools.h"
#include "router.h"
#include "db.h"

#define MESSAGE_QUE_LEN 100

void mes_init();
int mes_in(int client_fd, int t, int num, char* (pdata)[], int datalen[]);
int mes_out(int cliend_fd, int id);

#endif /* MESSAGE_H_ */
