/*
 * online.h
 *
 *  Created on: Oct 28, 2017
 *      Author: windroid
 */

#ifndef ONLINE_H_
#define ONLINE_H_

#include <pthread.h>
#include <set>
#include "tools.h"
#include "db.h"
#include "router.h"

#define BUFF_LEN 4096

int onl_init();
bool isOnline(int id);
void onl_add(int id);
void onl_del(int id);
int onl_query(int client_fd);


#endif /* ONLINE_H_ */
