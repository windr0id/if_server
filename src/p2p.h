/*
 * p2p.h
 *
 *  Created on: Nov 30, 2017
 *      Author: windroid
 */

#ifndef P2P_H_
#define P2P_H_


void p2p_in(int client_fd, int num, char* (pdata)[], int datalen[]);
int p2p_out(int client_fd);

#endif /* P2P_H_ */

