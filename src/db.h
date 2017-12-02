/*
 * db.h
 *
 *  Created on: Oct 27, 2017
 *      Author: windroid
 */

#ifndef DB_H_
#define DB_H_

int db_init();
int db_insert_user(char* username, char* password);
int db_check_user(int id, char* password);
int db_get_username(int id, char* out_username);


#endif /* DB_H_ */
