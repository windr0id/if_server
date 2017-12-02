/*
 * head.h
 *
 *  Created on: Dec 1, 2017
 *      Author: windroid
 */

#ifndef HEAD_H_
#define HEAD_H_

#define PORT 9762
#define BUFF_LEN 4096
#define MAX_DATA_NUM 24

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>

#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <mysql.h>

#include "log.h"
#include "tools.h"
#include "router.h"
#include "db.h"
#include "sign.h"
#include "message.h"
#include "msocket.h"
#include "online.h"
#include "p2p.h"

#endif /* HEAD_H_ */
