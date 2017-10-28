/*
 * sign.h
 *
 *  Created on: Oct 26, 2017
 *      Author: windroid
 */

#ifndef SIGN_H_
#define SIGN_H_

#include <string.h>
#include "tools.h"
#include "router.h"

int sign_login(int client_fd, int num, char* (pdata)[], int datalen[]);
int sign_up(int client_fd, int num, char* (pdata)[], int datalen[]);

#endif /* SIGN_H_ */
