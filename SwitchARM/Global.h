/*
 * Global.h
 *
 *  Created on: 2014��3��22��
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

// �豸 ID ��
extern char *G_DeviceID;

// ����֡���
extern time_t G_CommandID;

// ����� ID ��
extern char *G_DepartmentID;

// ������ IP ��ַ
extern char *G_ServerIPAddress;

// �������˿ں�
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
	char	*RS485FileName;			// RS485 �豸�ļ���
	int		RS485BaudRate;			// RS485 ������
	int		RS485DataBits;			// RS485 ����λ
	char	RS485Parity;			// RS485 ��żУ��
	int		RS485StopBits;			// RS485 ֹͣλ

	int		RS485QueryInterval;		// RS485 ���ݲ�ѯʱ��
	int		UploadInterval;			// �����ϴ�ʱ��

	BOOL	AliveEnable;			// ����������״̬
	int		AliveInterval;			// ���������ͼ��ʱ��
	char	*AliveMessage;			// ������������Ϣ

	BOOL	RealTimeUploadEnable;	// ʵʱ�ϴ�����״̬
	int		RealTimeUploadTotal;	// ʵʱ�ϴ���ʱ�䣨��λ���룩
	int		RealTimeUploadInterval;	// ʵʱ�ϴ����ʱ�䣨��λ���룩

	int		RelaysResetTimeBaseInterval;	// �̵���������λʱ�䣨��λ��΢�
	Relays	*R;								// �̵�����GPIO���������
} GlobalConfig;

extern GlobalConfig GC;

// ��ʼ��
extern void InitGlobalConfig(GlobalConfig *gc);
extern void Init();

// �����ϴ����ʱ��
extern int G_UploadInterval;

// ��·������λ 1
extern DWORD G_BreakerBits_1;
// ��·������λ 2
extern DWORD G_BreakerBits_2;

// �������ݰ�������
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
