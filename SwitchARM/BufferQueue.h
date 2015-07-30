/*
 * BufferQueue.h
 *
 *  Created on: 2014Äê6ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef BUFFERQUEUE_H_
#define BUFFERQUEUE_H_

typedef struct BufferQueue
{
	cJSON *Data;
	char *TxBuf;
	BOOL Valid;

	struct BufferQueue *Prev;
	struct BufferQueue *Next;
} BufferQueue;

extern BufferQueue *BufferQueue_Create(cJSON *data);
extern void BufferQueue_Delete(BufferQueue *bq);
extern void BufferQueue_Push(BufferQueue **bq, cJSON *data);
extern void BufferQueue_Pop(BufferQueue **bq);
extern void BufferQueue_MultiPop(BufferQueue **bq, int len);
extern int BufferQueue_Length(BufferQueue *bq);
extern void BufferQueue_Print(BufferQueue *bq);

#endif /* BUFFERQUEUE_H_ */
