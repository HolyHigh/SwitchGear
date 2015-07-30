/*
 * Global.h
 *
 *  Created on: 2014年3月22日
 *      Author: Administrator
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "Units/cJSON.h"

#define TRUE  1
#define FALSE 0

typedef unsigned char	BOOL;
typedef unsigned char	BYTE;
typedef unsigned int	DWORD;
typedef unsigned short	INT16U;

#define INT16HighByte(A)	(BYTE)(A >> 8)
#define INT16LowByte(A)		(BYTE)(A)

// 设备 ID 号
extern char *G_DeviceID;

// 传输帧序号
extern time_t G_CommandID;

// 配电所 ID 号
extern char *G_DepartmentID;

// 服务器 IP 地址
extern char *G_ServerIPAddress;

// 服务器端口号
extern int G_ServerPort;

typedef struct Relays
{
	int		Channel;
	int		GPIO;
	BOOL	Default;
	BOOL	ResetValid;
	int		ResetTime;

	int		ResetRemain;

	struct Relays	*Prev;
	struct Relays	*Next;
} Relays;

typedef struct GlobalConfig
{
	char	*RS485FileName;			// RS485 设备文件名
	int		RS485BaudRate;			// RS485 波特率
	int		RS485DataBits;			// RS485 数据位
	char	RS485Parity;			// RS485 奇偶校验
	int		RS485StopBits;			// RS485 停止位

	int		RS485QueryInterval;		// RS485 数据查询时间
	int		UploadInterval;			// 数据上传时间

	BOOL	AliveEnable;			// 心跳包激活状态
	int		AliveInterval;			// 心跳包发送间隔时间
	char	*AliveMessage;			// 心跳包发送信息

	BOOL	RealTimeUploadEnable;	// 实时上传激活状态
	int		RealTimeUploadTotal;	// 实时上传总时间（单位：秒）
	int		RealTimeUploadInterval;	// 实时上传间隔时间（单位：秒）

	int		RelaysResetTimeBaseInterval;	// 继电器基本复位时间（单位：微妙）
	Relays	*R;								// 继电器与GPIO口输出配置
} GlobalConfig;

extern GlobalConfig GC;

// 初始化
extern void InitGlobalConfig(GlobalConfig *gc);
extern void Init();

// 数据上传间隔时间
extern int G_UploadInterval;

// 断路器控制位 1
extern DWORD G_BreakerBits_1;
// 断路器控制位 2
extern DWORD G_BreakerBits_2;

// 网络数据包结束符
extern char *G_PacketEndTag;

extern unsigned long get_file_size(const char *path);
extern char * get_file_content(const char *path);
extern int SetSerialParams(int fd, int nSpeed, int nBits, char nEvent, int nStop);

extern float ByteToFloat(BYTE Byte1, BYTE Byte2, BYTE Byte3, BYTE Byte4);

extern char * my_localtime(time_t * time_input);
extern void SetLocalTime(int year, int month, int day, int hour, int minute, int second);

extern INT16U CRC16(const BYTE *ptr, int len);
extern INT16U crc_cal16 (const BYTE *ptr, int num);

extern void PrintBuffer(const BYTE *ptr, int len);

extern void PrintcJSON(const cJSON *json);

#endif /* GLOBAL_H_ */
