/*
 * Search.c
 *
 *  Created on: 2014年6月10日
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

#include "Units/cJSON.h"
#include "Global.h"
#include "Network.h"
#include "SearchDefine.h"
#include "Drivers/em9280_GPIO.h"
#include "Search.h"

SearchDefine *Search;

BOOL iswork = TRUE, isJustBoot = TRUE;
time_t TimeCurrent = 0, TimeUI = 0, TimeSwitch = 0;

void InitSearch()
{
	ReadSearchDefine("/mnt/nandflash/SearchDefine.json", &Search);
	PackageAllTxBuffer(Search);
}

cJSON * CreateSamplesJsonObject(const char *switchgearID, const Sample *sm, const BYTE *buffer)
{
	cJSON *samples = NULL;
	char buf[30];
	int index = 0;

	if (switchgearID && sm && buffer)
	{
		samples = cJSON_CreateObject();
		cJSON_AddStringToObject(samples, "SID", switchgearID);

		Sample *smTemp = (Sample *)sm;

		while (smTemp)
		{
			switch (smTemp->ValueType)
			{
			case SampleValueType_Float:
				sprintf(buf, "%.2f"
						, ByteToFloat(buffer[index], buffer[index + 1], buffer[index + 2], buffer[index + 3]));
				cJSON_AddStringToObject(samples, smTemp->Name, buf);
				index += 4;
			break;
			case SampleValueType_Bit:
				cJSON_AddNumberToObject(samples, smTemp->Name, buffer[index + 1] ? 1 : 0);
				index += 2;
			break;
			default:
			break;
			}

			smTemp = smTemp->Next;
		}
	}

	return samples;
}

cJSON * CreateCurrentSamplesJson(const char *departmentID
		, const char *switchgearID, const Sample *sm, const BYTE *buffer)
{
	cJSON *root = NULL, *samples = NULL, *samplesArray = NULL;
	time_t tmptime;
	char sCMDID[20];

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "TP", "Up");
	sprintf(sCMDID, "%ld", G_CommandID);
	cJSON_AddStringToObject(root, "CID", sCMDID);
	time(&tmptime);
	cJSON_AddStringToObject(root, "TM", my_localtime(&tmptime));
	cJSON_AddStringToObject(root, "DID", departmentID);
	cJSON_AddStringToObject(root, "SID", switchgearID);

	samplesArray = cJSON_CreateArray();

	samples = CreateSamplesJsonObject(switchgearID, sm, buffer);
	cJSON_AddItemToArray(samplesArray, samples);

	cJSON_AddItemToObject(root, "SGS", samplesArray);

#ifdef DEBUG0
	PrintcJSON(root);
#endif

	return root;
}

cJSON *CreateCurrentTotalSamplesJson(const char *departmentID
		, const char *switchgearID, const SearchDefine *sd)
{
	cJSON *root = NULL, *samples = NULL, *samplesArray = NULL;
	time_t tmptime;
	char sCMDID[20];

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "TP", "Up");
	sprintf(sCMDID, "%ld", G_CommandID);
	cJSON_AddStringToObject(root, "CID", sCMDID);
	time(&tmptime);
	cJSON_AddStringToObject(root, "TM", my_localtime(&tmptime));
	cJSON_AddStringToObject(root, "DID", departmentID);
	cJSON_AddStringToObject(root, "SID", switchgearID);

	samplesArray = cJSON_CreateArray();

	Meter *mt = sd->Meters;
	while (mt)
	{
		samples = CreateSamplesJsonObject(switchgearID, mt->Samples, mt->PackageBuffer.RxBuffer + 3);
		cJSON_AddItemToArray(samplesArray, samples);

		mt = mt->Next;
	}

	cJSON_AddItemToObject(root, "SGS", samplesArray);

#ifdef DEBUG0
	PrintcJSON(root);
#endif

	return root;
}

cJSON * CreateAlivePacket(const char *departmentID
		, const char *switchgearID, const char *aliveMessage)
{
	cJSON *root = NULL;
	time_t tmptime;
	char sCMDID[20];

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "TP", "On");
	sprintf(sCMDID, "%ld", G_CommandID);
	cJSON_AddStringToObject(root, "CID", sCMDID);
	time(&tmptime);
	cJSON_AddStringToObject(root, "TM", my_localtime(&tmptime));
	cJSON_AddStringToObject(root, "DID", departmentID);
	cJSON_AddStringToObject(root, "SID", switchgearID);
	cJSON_AddStringToObject(root, "MSG", (aliveMessage != NULL) ? aliveMessage : "");

#ifdef DEBUG0
	PrintcJSON(root);
#endif

	return root;
}

void thread_CommonSearch(const SearchDefine *sd)
{
	int ad_fd = 0, set_serial, readbytes, writebytes;
	FILE *file = NULL;
	int RS485QueryCounter 			= 0;
	int AliveCounter				= 0;
	int RealTimeUploadCounter		= 0;
	int RealTimeUploadTotalCounter	= 0;

	if (sd && sd->DepartmentID && sd->SwitchgearID && sd->Meters)
	{
		while(TRUE)
		{
			if (!ad_fd)
			{
				// 打开 485
				if ((ad_fd = open(GC.RS485FileName, O_RDWR | O_NONBLOCK)) == -1)
				{
					perror("\nFail to open serial!");
					sleep(2);
					continue;
				}

				if ((set_serial = SetSerialParams(ad_fd, GC.RS485BaudRate, GC.RS485DataBits, GC.RS485Parity, GC.RS485StopBits)) == -1)
				{
					perror("\nFail to set serial");
					sleep(2);
					continue;
				}
			}

			if (!file)
			{
				if ((file = fopen("./rs485out", "a+")) == NULL)
				{
					perror("\nFail to open file!");
				}
			}

			tcflush(ad_fd, TCOFLUSH);

			// 遍历 SearchDefine，根据所有 Meter 记录打包要发送的数据并发送
			Meter *mt = sd->Meters;
			while (mt)
			{
#ifdef	DEBUG0
				printf("\n【Tx】 0x");
				PrintBuffer(mt->PackageBuffer.TxBuffer, mt->PackageBuffer.TxLength);
				fflush(stdout);
#endif

				//mt->PackageBuffer.RxBufferValid = FALSE;
				writebytes = write(ad_fd, mt->PackageBuffer.TxBuffer, mt->PackageBuffer.TxLength);

				// 等待485数据上传，0.1s大概能收100字节（9600bps）
				usleep(100000);
				time(&TimeCurrent);

				if (writebytes <= 0)
				{
					if (errno == EINTR)
					{
						printf("\n[SeanSend]error errno==EINTR continue");
						continue;
					} else if (errno == EAGAIN)
					{
						printf("\n[SeanSend]error errno==EAGAIN continue");
						continue;
					} else
					{
						printf("\n[SeanSend]ERROR: errno = %d, strerror = %s" , errno, strerror(errno));
						return;
					}
				}

				memset(mt->PackageBuffer.RxBuffer, 0, mt->PackageBuffer.RxLength);
				readbytes = read(ad_fd, mt->PackageBuffer.RxBuffer, mt->PackageBuffer.RxLength);

				if (readbytes)
				{
#ifdef	DEBUG0
					printf("\n【Rx】 0x");
					PrintBuffer(mt->PackageBuffer.RxBuffer, readbytes);
					printf("\n");
					fflush(stdout);
#endif
					if((mt->PackageBuffer.RxBuffer[0] == mt->PackageBuffer.TxBuffer[0])
						&& (mt->PackageBuffer.RxBuffer[1] == mt->PackageBuffer.TxBuffer[1]))
					{
						// TODO: CRC 校验

						//mt->PackageBuffer.RxBufferValid = TRUE;
					}
				}

				mt = mt->Next;
			}

			/* 程序启动后立马发送首条数据，正常按照 GC.UploadInterval 时间间隔发送数据 */
			if (isJustBoot || (iswork && (RS485QueryCounter >= GC.UploadInterval)))
			{
				isJustBoot = FALSE;
				RS485QueryCounter = 0;

				SendData(CreateCurrentTotalSamplesJson(sd->DepartmentID, sd->SwitchgearID, sd));
			}

			if (GC.AliveEnable && NetworkConneted())
			{
				AliveCounter += GC.RS485QueryInterval;

				if (iswork && (AliveCounter >= GC.AliveInterval))
				{
					AliveCounter = 0;

					SendData(CreateAlivePacket(sd->DepartmentID, sd->SwitchgearID, GC.AliveMessage));
				}
			}

			if (GC.RealTimeUploadEnable)
			{
				RealTimeUploadCounter += GC.RS485QueryInterval;
				RealTimeUploadTotalCounter += GC.RS485QueryInterval;

				if (iswork && (RealTimeUploadCounter >= GC.RealTimeUploadInterval))
				{
					RealTimeUploadCounter = 0;

					SendData(CreateCurrentTotalSamplesJson(sd->DepartmentID, sd->SwitchgearID, sd));
				}

				if (RealTimeUploadTotalCounter >= GC.RealTimeUploadTotal)
				{
					RealTimeUploadTotalCounter = 0;

					GC.RealTimeUploadEnable = FALSE;
					GC.RealTimeUploadTotal = 0;
					GC.RealTimeUploadInterval = 0;
				}
			}

			sleep(GC.RS485QueryInterval);
			RS485QueryCounter += GC.RS485QueryInterval;
		}

		close(ad_fd);
	}
}

