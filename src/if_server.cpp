#include "msocket.h"

#define PORT 9762

int main(){
	db_init();//初始化数据库连接
	mes_init();//初始化消息
	socket_init();//初始化socket
	socket_wait();

    return 0;
}
