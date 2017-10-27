/*
 * db.h
 *
 *  Created on: Oct 27, 2017
 *      Author: windroid
 */

#ifndef DB_H_
#define DB_H_

#include <string>
#include <sstream>
#include <mysql.h>
#include "log.h"

int db_init();
int db_sign_up(char* username, char* password);

#endif /* DB_H_ */
