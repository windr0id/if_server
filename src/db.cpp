/*
 * db.cpp
 *
 *  Created on: Oct 27, 2017
 *      Author: windroid
 */

#include "db.h"

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

int db_query(string sql){
	const char* c_sql = sql.c_str();
	if (mysql_query(conn, c_sql) != 0) {
		log("db=>query error: ", c_sql, mysql_errno(conn));
		return -1;
	}
	return 0;
}

int db_insert_user(char* username, char* password){
	MYSQL_RES *res;
	MYSQL_ROW row;
	string sql = "SELECT max(id) FROM user";
	db_query(sql);
	res = mysql_use_result(conn);
	row = mysql_fetch_row(res);
	int nid = atoi(row[0])+1;
	mysql_free_result(res);
	stringstream ss;
	ss<<"INSERT INTO user(`id`,`username`,`password`)VALUES("<<nid<<",'"<<username<<"','"<<password<<"')";
	log(ss.str().c_str());
	if(db_query(ss.str()) != 0){
		return -1;
	}else{
		return nid;
	}
}




