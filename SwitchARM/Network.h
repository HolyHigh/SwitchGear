/*
 * Network.h
 *
 *  Created on: 2014Äê3ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "BufferQueue.h"

// for CodeTest
extern BufferQueue *NetworkBuf;

extern BOOL NetworkConneted();
extern void SendData(cJSON * data);
extern void *thread_network(void *arg);

extern void *thread_TimerDriveRelays(void *);

#endif /* NETWORK_H_ */
