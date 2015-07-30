#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>


#include "Units/cJSON.h"
#include "Drivers/em9280_drivers.h"
#include "Drivers/em9280_GPIO.h"
#include "Global.h"
#include "BufferQueue.h"

#include "Network.h"

pthread_mutex_t mutex;

#define NET_TIMEOUT    5

BOOL NeedAnswer = 0, IsSend = 0, WaitingAnswer = FALSE;
static int timeout_count = 0;

static int sockfd = 0, connectsockfd = 0;

BufferQueue *NetworkBuf = NULL;
#define NETWORKBUF_MAXLEN	100

BOOL NetworkConneted()
{
	return (sockfd > 0);
}

void SendData(cJSON *data)
{
	pthread_mutex_lock(&mutex);
	BufferQueue_Push(&NetworkBuf, data);
	pthread_mutex_unlock(&mutex);

	if (BufferQueue_Length(NetworkBuf) > NETWORKBUF_MAXLEN)
	{
		BufferQueue_MultiPop(&NetworkBuf, NETWORKBUF_MAXLEN / 2);
	}

#ifdef DEBUG
	printf("\n\n【SendData】");
	PrintcJSON(data);
	fflush(stdout);
#endif

	NeedAnswer = TRUE;
	IsSend = TRUE;
	timeout_count = 0;
	G_CommandID++;
}

void FinishSend(BOOL bSuc)
{
	IsSend = FALSE;

	if (bSuc)
	{
		WaitingAnswer = TRUE;
	}
}

void FinishAnswer()
{
	NeedAnswer = FALSE;
	WaitingAnswer = FALSE;
	timeout_count = 0;

	if (!NetworkBuf->Valid)
	{
#ifdef DEBUG0
		printf("\n\n【POP-:<】");
		fflush(stdout);
#endif
		BufferQueue_Pop(&NetworkBuf);
	}
}

static void ClearSocket(int *psockfd)
{
	IsSend = FALSE;
	NeedAnswer = FALSE;
	WaitingAnswer = FALSE;

	if (*psockfd)
	{
		close(*psockfd);
		*psockfd = 0;
	}
}

void GPIOControl(int channel, int mode)
{
	DWORD dwBits = 1;

	switch (channel)
	{
	case 1:
		dwBits = GPIO0;
		break;
	case 2:
		dwBits = GPIO1;
		break;
	default:
		return;
	}

	GPIO_Out(dwBits, TRUE);
	usleep(900000);
	GPIO_Out(dwBits, FALSE);
}

void RunRealTimeUpload(int total, int interval)
{
	GC.RealTimeUploadEnable = TRUE;
	GC.RealTimeUploadTotal = total;
	GC.RealTimeUploadInterval = interval;
}

static void read_response(char *buffer)
{
	cJSON *res, *item, *item2;

	if (!(res = cJSON_Parse(buffer)))
	{
#ifdef DEBUG
		printf("\nResponse format error!");
		fflush(stdout);
#endif
		return;
	}

	if ((item = cJSON_GetObjectItem(res, "TP")))
	{
		if ((strcasecmp(item->valuestring, "Ans") == 0))
		{
			// 响应数据包
			if ((item = cJSON_GetObjectItem(res, "CID")))
			{
				if (!NetworkBuf || !NetworkBuf->Data)
				{
#ifdef DEBUG
					printf("\nAnswer too late!");
					fflush(stdout);
#endif
					cJSON_Delete(res);
					return;
				}

				item2 = cJSON_GetObjectItem(NetworkBuf->Data, "CID");
				if (strcasecmp(item->valuestring, item2->valuestring) == 0)
				{
					// read success
					item = cJSON_GetObjectItem(res, "RES");
					if (item && (strcasecmp(item->valuestring, "Success") == 0))
					{
						// success
#ifdef DEBUG
						printf("\nAnswer success");
						fflush(stdout);
#endif
					}
					else
					{
						printf("\nAnswer fail");
						if ((item = cJSON_GetObjectItem(res, "RSN")))
						{
#ifdef DEBUG
							printf(", reason=%s", item->valuestring);
							fflush(stdout);
#endif
							// TODO: log file
						}
					}

					NetworkBuf->Valid = FALSE;
					FinishAnswer();
				}
				else {
					// other data
#ifdef DEBUG
					printf("\nCID=%s, %s", item->valuestring, item2->valuestring);
					fflush(stdout);
#endif
				}
			}
			else {
				// error packet
			}
		}
		else if ((strcasecmp(item->valuestring, "Ctrl") == 0))
		{
			// 控制数据包
			if ((item = cJSON_GetObjectItem(res, "CH")))
			{
				int ch = item->valueint;
				if ((item = cJSON_GetObjectItem(res, "MD")))
				{
					int mode = item->valueint;

#ifdef DEBUG
					printf("\n[GPIO Output: %d, %d]", ch, mode);
					fflush(stdout);
#endif
					GPIOControl(ch, mode);
				}
			}
		}
		else if ((strcasecmp(item->valuestring, "Query") == 0))
		{
			// TODO: 配电所+开关柜序号检查，防止误操作

			// 实时查询数据包
			if ((item = cJSON_GetObjectItem(res, "TTL")))
			{
				int total = item->valueint;
				if ((item = cJSON_GetObjectItem(res, "INT")))
				{
					int interval = item->valueint;

#ifdef DEBUG
					printf("\n[RealTimeUpload: %d, %d]", total, interval);
					fflush(stdout);
#endif

					RunRealTimeUpload(total, interval);
				}
			}
		}
		else {
			// error packet
#ifdef DEBUG
			printf("\nUnknown packet!");
			fflush(stdout);
#endif
		}
	}

	cJSON_Delete(res);
}

#define NETWORK_RecvBufLenMax	1024

void *thread_network(void *arg)
{
	struct timeval timeset;
	fd_set readfd;
	int i, n, maxfd, result, ret;
	struct sockaddr_in s;
	char recv[256], RecvBuf[NETWORK_RecvBufLenMax];
	char *endTagIndex = NULL;
	char tmpbuf[1024];
	int PacketEndTagLen = strlen(G_PacketEndTag);

	pthread_mutex_init(&mutex, NULL);

	memset(recv, '\0', sizeof(recv));
	memset(RecvBuf, '\0', sizeof(RecvBuf));

	// Read socket timeout time
	timeset.tv_sec = 0;
	timeset.tv_usec = 100000;

	/////////////////////////////////////////////////
	// 忽略 SIGPIPE 信号
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );
	/////////////////////////////////////////////////

	while (TRUE)
	{
		// 网络线程 50毫秒 执行一次
		usleep(50000);

#ifdef DEBUG0
		printf("\n[Network Thread] Alive...");
		fflush(stdout);
#endif

		////////////////////////////////////////////////////////////////////////
		// Reconnect
		if (!sockfd)
		{
			if ((connectsockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				sockfd = 0;
				continue;
			}

			memset(&s, 0, sizeof(struct sockaddr_in));

			// 设置服务器信息
			s.sin_family = AF_INET;
			s.sin_addr.s_addr = inet_addr(G_ServerIPAddress);
			s.sin_port = htons(G_ServerPort);

			// 连接到服务器端
			if ((connect(connectsockfd, (struct sockaddr *) &s, sizeof(s))) == -1)
			{
				ClearSocket(&connectsockfd);
				continue;
			} else
			{
				sockfd = connectsockfd;
			}
		}
		////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// 读取 socket 数据
		if (sockfd)
		{
			FD_ZERO(&readfd);
			FD_SET(sockfd, &readfd);
			maxfd = sockfd + 1;		// 句柄集合最大值 + 1
			result = select(maxfd, &readfd, NULL, NULL, &timeset);

			if (result < 0)
			{
				// Error
				ClearSocket(&sockfd);
				continue;
			}
			else if (result == 0)
			{
				// Timeout
				// continue;
			}
			else if (FD_ISSET(sockfd, &readfd))
			{
				ret = read(sockfd, &recv, sizeof(recv));

				if (ret > 0)
				{
					for (i = 0; i < ret; i++)
					{
						if (recv[ret] == '\0')
						{
							recv[ret] = ' ';
						}
					}
					recv[ret] = '\0';

#ifdef DEBUG
					printf("\n[Network Thread] Recv = %s", recv);
					fflush(stdout);
#endif

					if ((endTagIndex = strstr(recv, G_PacketEndTag)))
					{
						// 收到包结束符
						strncat(RecvBuf, recv, endTagIndex - recv);

#ifdef DEBUG
						printf("\n[Network Thread] RecvPacket = %s", RecvBuf);
						fflush(stdout);
#endif
						read_response(RecvBuf);

						memset(RecvBuf, '\0', strlen(RecvBuf));
						strcat(RecvBuf, endTagIndex + PacketEndTagLen);
					} else
					{
						n = strlen(RecvBuf) + strlen(recv) - NETWORK_RecvBufLenMax + 1;
						if (n >= 0)
						{
							memset(RecvBuf, '\0', n);
							strcat(RecvBuf, RecvBuf + n + 1);
						}
						strcat(RecvBuf, recv);
					}
				}
			}
		}
		////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		// 发送 socket 数据
		if (sockfd)
		{
			if (!WaitingAnswer && NetworkBuf && NetworkBuf->TxBuf)
			{
				// 通过 TCP方式发送，保持 TCP连接不断开
				sprintf(tmpbuf, "%s%s", NetworkBuf->TxBuf, G_PacketEndTag);
				if (send(sockfd, tmpbuf, strlen(tmpbuf), 0) <= 0)
				{
					// 本地发送失败时，恢复网络状态重新建立网络连接
					ClearSocket(&sockfd);
					FinishSend(FALSE);
					FinishAnswer();
					continue;
				}
				else
				{
					// 数据包发送成功
#ifdef DEBUG
					printf("\n[Network Thread] SendPacket = %s", NetworkBuf->TxBuf);
					fflush(stdout);
#endif
					FinishSend(TRUE);
				}
			}
		}

		if (NeedAnswer && WaitingAnswer)
		{
			// 等待响应超时处理
			timeout_count++;
			if (timeout_count >= NET_TIMEOUT)
			{
				// 等待响应数据超时，认为网络断开，则复位重新连接网络
				ClearSocket(&sockfd);
				FinishAnswer();
			}
		}
		////////////////////////////////////////////////////////////////////////
	}
	
	return NULL;
}
/*
int TimerDriveRelaysInterval	= 50000;	// 单位：us

void SetTimerDriveRelays(int index)
{
	RelaysArray.Timer[index] = TimerDriveRelaysCount;
}

void *thread_TimerDriveRelays(void *arg)
{
	Relays *R = (Relays *)arg;
	int i = 0;

	while (TRUE)
	{
		usleep(TimerDriveRelaysInterval);

		for (i = 0; i < RelaysCountMax; i++)
		{
			if (R->Timer[i] > 0)
			{
				R->Timer[i]--;

				if (R->Timer[i] <= 0)
				{
					// 复位继电器
					GPIO_Out(R->GPIO[i], !(R->GPIOValidStatus[i]));
				}
			}
		}
	}

	return NULL;
}
*/
