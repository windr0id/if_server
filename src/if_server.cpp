#include "info.h"

#define PORT 9762

char buff[4096];



int main(){
	log("in");
	char b[30] = {0};
	char* p1 = b;
	printf("%d\n", p1);
	char* p2 = &b[20];
	printf("%d\n", p2);
	log("out");
	socket_init();

	socket_wait();


    return 0;
}
