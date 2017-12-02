/*
 * db.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: windroid
 */

#include "head.h"

using namespace std;

MYSQL *conn;

int db_init(){
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, "localhost", "root", "qwas12", "if", 0, NULL, 0)) {
		log("db=>init error.");
		return -1;
	}
	return 0;
}

int db_query(const char *c_sql){
	if (mysql_query(conn, c_sql) != 0) {
		log("db=>query error: ", c_sql, mysql_errno(conn));
		return -1;
	}
	//log(c_sql);
	return 0;
}

int db_insert_user(char* username, char* password){
	MYSQL_RES *res;
	MYSQL_ROW row;
	char sql[255] = "SELECT max(id) FROM user";
	db_query(sql);
	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);
	int nid = atoi(row[0])+1;
	mysql_free_result(res);
	stringstream ss;
	ss<<"INSERT INTO user(`id`,`username`,`password`)VALUES("<<nid<<",'"<<username<<"','"<<password<<"')";
	if(db_query(ss.str().c_str()) != 0){
		return -1;
	}else{
		return nid;
	}
}

int db_check_user(int id, char* password){
	stringstream ss;
	ss<<"SELECT count(*) FROM user where id = "<<id<<" and password = '"<<password<<"'";
	MYSQL_RES *res;
	MYSQL_ROW row;
	if(db_query(ss.str().c_str()) != 0){
		return -1;
	}else{
		res = mysql_use_result(conn);
		row = mysql_fetch_row(res);
		int count = atoi(row[0]);
		mysql_free_result(res);
		if(count == 0){
			log("db_check_user: userinfo incorrect.");
			return -2;
		}else{
			return 0;
		}
	}
}

int db_get_username(int id, char* out_username){
	stringstream ss;
	ss<<"SELECT username FROM user where id = "<<id;
	MYSQL_RES *res;
	MYSQL_ROW row;
	if(db_query(ss.str().c_str()) != 0){
		return -1;
	}
	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);
	memcpy(out_username, row[0], strlen(row[0])+1);
	mysql_free_result(res);
	return 0;
}


