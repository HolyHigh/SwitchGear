/*
 * Global.c
 *
 *  Created on: 2014年3月22日
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include "Units/cJSON.h"
#include "Global.h"

///////////////////////////////////////////////////////////////////////////////////
//BEGIN 全局变量

/*
 * 设备 ID 号
 */
char *G_DeviceID;

/*
 * 传输帧序号
 */
time_t G_CommandID;

/*
 * 配电所 ID 号
 */
char *G_DepartmentID;

/*
 * 服务器 IP 地址
 */
char *G_ServerIPAddress;

/*
 * 服务器端口号
 */
int G_ServerPort;

/*
 * 全局配置
 */
GlobalConfig GC;

/*
 * 数据上传间隔时间，默认值 5秒
 */
int G_UploadInterval = 5;

/*
 * 网络数据包结束符
 */
char *G_PacketEndTag;

//END 全局变量
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
//BEGIN 全局方法

void InitGlobalConfig(GlobalConfig *gc)
{
	if (gc)
	{
		gc->RS485FileName	= "/dev/ttyS3";
		gc->RS485BaudRate	= 9600;
		gc->RS485DataBits	= 8;
		gc->RS485Parity		= 'N';
		gc->RS485StopBits	= 1;

		gc->RS485QueryInterval	= 1;

		gc->AliveEnable		= TRUE;
		gc->AliveInterval	= 60;
		gc->AliveMessage	= NULL;

		gc->RealTimeUploadEnable	= FALSE;
		gc->RealTimeUploadTotal		= 0;
		gc->RealTimeUploadInterval	= 0;

		gc->RelaysResetTimeBaseInterval	= 50000;
		gc->R							= NULL;
	}
}

void Init()
{
	char *buf;
	cJSON *root, *item;

	G_CommandID = time(&G_CommandID);

	timezone = 8*60*60;

	if ((buf = get_file_content("/mnt/nandflash/config.json")) == NULL)
	{
		printf("Cannot open config.json file!\n");
		return;
	}

	if ((root = cJSON_Parse(buf)) == NULL)
	{
		printf("Cannot read config.json file!\n");
		return;
	}

	if ((item = cJSON_GetObjectItem(root, "DeviceID")) != NULL)
	{
		G_DeviceID = malloc(strlen(item->valuestring));
		strncpy(G_DeviceID, item->valuestring, strlen(item->valuestring));
		printf("\nDeviceID: %s", G_DeviceID);
	}

	if ((item = cJSON_GetObjectItem(root, "DepartmentID")) != NULL)
	{
		G_DepartmentID = malloc(strlen(item->valuestring));
		strncpy(G_DepartmentID, item->valuestring, strlen(item->valuestring));
		printf("\nDepartmentID: %s", G_DepartmentID);
	}

	if ((item = cJSON_GetObjectItem(root, "ServerIP")) != NULL)
	{
		G_ServerIPAddress = malloc(strlen(item->valuestring));
		strncpy(G_ServerIPAddress, item->valuestring, strlen(item->valuestring));
		printf("\nServerIP: %s", G_ServerIPAddress);
	}

	if ((item = cJSON_GetObjectItem(root, "ServerPort")) != NULL)
	{
		G_ServerPort = item->valueint;
		printf("\nServerPort: %d", G_ServerPort);
	}

	if ((item = cJSON_GetObjectItem(root, "RS485FileName")) != NULL)
	{
		GC.RS485FileName = strdup(item->valuestring);
		printf("\nRS485FileName: %s", GC.RS485FileName);
	}

	if ((item = cJSON_GetObjectItem(root, "RS485BaudRate")) != NULL)
	{
		GC.RS485BaudRate = item->valueint;
		printf("\nRS485BaudRate: %d", GC.RS485BaudRate);
	}

	if ((item = cJSON_GetObjectItem(root, "RS485QueryInterval")) != NULL)
	{
		GC.RS485QueryInterval = item->valueint;
		printf("\nRS485QueryInterval: %d", GC.RS485QueryInterval);
	}

	if ((item = cJSON_GetObjectItem(root, "UploadInterval")) != NULL)
	{
		GC.UploadInterval = item->valueint;
		printf("\nUploadInterval: %d", GC.UploadInterval);
	}

	if ((item = cJSON_GetObjectItem(root, "PacketEndTag")) != NULL)
	{
		G_PacketEndTag = malloc(strlen(item->valuestring));
		strncpy(G_PacketEndTag, item->valuestring, strlen(item->valuestring));
		printf("\nPacketEndTag: %s", G_PacketEndTag);
	}

	if ((item = cJSON_GetObjectItem(root, "AliveEnable")) != NULL)
	{
		GC.AliveEnable = (item->type == cJSON_True);
		printf("\nAliveEnable: %d", GC.AliveEnable);
	}

	if ((item = cJSON_GetObjectItem(root, "AliveInterval")) != NULL)
	{
		GC.AliveInterval = item->valueint;
		printf("\nAliveInterval: %d", GC.AliveInterval);
	}

	if ((item = cJSON_GetObjectItem(root, "AliveMessage")) != NULL)
	{
		if (strlen(item->valuestring))
		{
			GC.AliveMessage = malloc(strlen(item->valuestring));
			strncpy(GC.AliveMessage, item->valuestring, strlen(item->valuestring));
		}
		printf("\nAliveMessage: %s", GC.AliveMessage);
	}

	free(buf);
	cJSON_Delete(root);
}

unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;

    if(stat(path, &statbuff) < 0)
    {
        return filesize;
    }else
    {
        filesize = statbuff.st_size;
    }

    return filesize;
}

char * get_file_content(const char *path)
{
	unsigned long filesize = 0;
	char * buf = NULL;
	FILE * fl = NULL;

	filesize = get_file_size(path);
	if (filesize <= 0)
		return NULL;
	buf = malloc(filesize + 1);
	if (buf==NULL)
		return NULL;
	fl = fopen(path, "rt");
	if (fl==NULL)
		return NULL;
	fread(buf, filesize, 1, fl);
	fclose(fl);
	buf[filesize] = '\0';
	return buf;
}

int SetSerialParams(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;

    if (tcgetattr(fd, &oldtio) != 0)
    {
    	perror("SetupSerial 1");
    	return -1;
    }

    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
    case 5:
		newtio.c_cflag |= CS5;
		break;
    case 6:
		newtio.c_cflag |= CS6;
		break;
    case 7:
        newtio.c_cflag |= CS7;
        break;
    default:
        newtio.c_cflag |= CS8;
    }

    switch( nEvent )
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    default:
        newtio.c_cflag &= ~PARENB;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
    }

    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
    	newtio.c_cflag |=  CSTOPB;

    newtio.c_cc[VTIME]  = 0;//重要
    newtio.c_cc[VMIN] = 100;//返回的最小值  重要
    tcflush(fd, TCIFLUSH);
    if((tcsetattr(fd, TCSANOW, &newtio)) != 0)
    {
        perror("com set error");
        return -1;
    }

    return 0;
}

float ByteToFloat(BYTE Byte1, BYTE Byte2, BYTE Byte3, BYTE Byte4)
{
	int bits = (Byte1 << 24) + (Byte2 << 16) + (Byte3 << 8) + Byte4;

	//s 为符号（sign）；e 为指数（exponent）；m 为有效位数（mantissa）
    int
    s = (bits >> 31) ? -1 : 1,
    e = (bits >> 23) & 0xff,
    m = (e == 0) ? ((bits & 0x7fffff) << 1) : ((bits & 0x7fffff) | 0x800000);

    return s * m * (float)pow(2, e - 150);
}

char * my_localtime(time_t * time_input)
{
	struct tm * tmptime;
	static char buf[100];

	tmptime = localtime(time_input);
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
			tmptime->tm_year+1900, tmptime->tm_mon+1, tmptime->tm_mday, tmptime->tm_hour, tmptime->tm_min, tmptime->tm_sec);
	return buf;
}

void SetLocalTime(int year, int month, int day, int hour, int minute, int second)
{
	struct tm mytm;
	time_t t;

	if (!year)
		return;

	mytm.tm_year	= year - 1900;
	mytm.tm_mon		= month - 1;
	mytm.tm_mday	= day;
	mytm.tm_hour	= hour;
	mytm.tm_min		= minute;
	mytm.tm_sec		= second;

	printf("\n%04d-%02d-%02d %02d:%02d:%02d\n", mytm.tm_year + 1900,
		mytm.tm_mon + 1, mytm.tm_mday, mytm.tm_hour, mytm.tm_min, mytm.tm_sec);

	t = mktime(&mytm) + 8 * 60 * 60;
	stime(&t);
	system("hwclock -w");
}

// 查表法计算CRC16
INT16U CRC16(const BYTE *ptr, int len)
{
	/* CRC余式表 */
	static INT16U CRC16Table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
	};

	INT16U crc = 0xFFFF;

	while(len-->0)
	{
		crc = CRC16Table[(BYTE)(crc ^ *ptr++)] ^ (crc >> 8);
	}

	return(~crc);
}

INT16U crc_cal16 (const BYTE *ptr, int num)
{
	BYTE i, j;
	INT16U c, crc=0xFFFF;

	for(i = 0; i < num; i ++)
	{
		c = ptr[i] & 0x00FF;
		crc ^= c;
		for(j = 0;j < 8; j ++)
		{
			if (crc & 0x0001)
			{
				crc>>=1;
				crc^=0xA001;
			}
			else crc>>=1;
		}
	}

	return(crc);
}

void PrintBuffer(const BYTE *ptr, int len)
{
	int i;

	if (ptr)
	{
		for (i = 0; i < len; i++)
		{
			printf("%02X ", *(ptr + i));
		}
	}
}

void PrintcJSON(const cJSON *json)
{
	printf("\n【JSON】: %s", cJSON_PrintUnformatted((cJSON *)json));
}

//END 全局方法
///////////////////////////////////////////////////////////////////////////////////
