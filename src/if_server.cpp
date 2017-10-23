#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include "log.h"
#include "info.h"

#define PORT 9762

char buff[4096];

int main(){
    init();
    wait();

    return 0;
}
