/*
 * BufferQueue.c
 *
 *  Created on: 2014Äê6ÔÂ10ÈÕ
 *      Author: Administrator
 */

#include <string.h>

#include "Global.h"
#include "BufferQueue.h"

//////////////////////////////////////////////////////////////////////////////////////
//
/* Internal constructor. */
BufferQueue *BufferQueue_Create(cJSON *data)
{
	BufferQueue *node = (BufferQueue *)malloc(sizeof(BufferQueue));
	if (node)
	{
		memset(node, 0, sizeof(BufferQueue));
		node->Data	= data;
		node->TxBuf	= cJSON_PrintUnformatted(data);
		node->Valid	= TRUE;
		node->Prev	= NULL;
		node->Next	= NULL;
	}
	return node;
}

/* Delete a BufferQueue structure. */
void BufferQueue_Delete(BufferQueue *bq)
{
	BufferQueue *next;
	while (bq)
	{
		next = bq->Next;
		if (bq->Data) cJSON_Delete(bq->Data);
		if (bq->TxBuf) free(bq->TxBuf);
		free(bq);
		bq = next;
	}
}

void BufferQueue_Push(BufferQueue **bq, cJSON *data)
{
	if (data)
	{
		BufferQueue *bq_temp = NULL;

		if ((bq_temp = BufferQueue_Create(data)))
		{
			if (!*bq)
			{
				*bq = bq_temp;
			} else
			{
				BufferQueue *bq_t = *bq;
				while (bq_t->Next) bq_t = bq_t->Next;
				bq_t->Next = bq_temp;
				bq_temp->Prev = bq_t;
			}
		}
	}
}

void BufferQueue_Pop(BufferQueue **bq)
{
	if (*bq)
	{
		BufferQueue *bq_temp = *bq;
		*bq = (*bq)->Next;
		if (*bq) (*bq)->Prev = NULL;

		bq_temp->Next = NULL;
		BufferQueue_Delete(bq_temp);
	}
}

void BufferQueue_MultiPop(BufferQueue **bq, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		BufferQueue_Pop(bq);
	}
}

int BufferQueue_Length(BufferQueue *bq)
{
	int len = 0;

	while (bq)
	{
		len++;
		bq = bq->Next;
	}

	return len;
}

void BufferQueue_Print(BufferQueue *bq)
{
	while (bq)
	{
		printf("\n[BufferQueue: %s]", bq->TxBuf);
		fflush(stdout);

		bq = bq->Next;
	}
}
