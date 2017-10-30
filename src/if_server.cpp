#include "msocket.h"

#define PORT 9762

int main(){
	db_init();//初始化数据库连接
	mes_init();//初始化消息队列
	onl_init();//初始化在线用户队列
	sign_init();//初始化鉴权队列
	socket_init();//初始化socket
	socket_wait();

    return 0;
}
