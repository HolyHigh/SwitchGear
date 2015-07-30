/*
 * SearchDefine.h
 *
 *  Created on: 2014年3月31日
 *      Author: Administrator
 */

#ifndef SEARCHDEFINE_H_
#define SEARCHDEFINE_H_

typedef struct Package
{
	BYTE TxLength;
	BYTE *TxBuffer;
	BYTE RxLength;
	BYTE *RxBuffer;
} Package;

typedef enum SampleValueType
{
	SampleValueType_Float,
	SampleValueType_Bit,
	SampleValueType_Unknown
} SampleValueType;

/*
typedef enum PackageStatus
{

} PackageStatus;
*/
typedef struct Sample
{
	char *Name;									// 采样点名称（用于上传时指定数据名称）
	SampleValueType ValueType;					// 采样点值类型
	float FloatValue;							// 存储浮点类型的值
	int IntValue;								// 存储整数类型的值

	struct Sample *Prev;
	struct Sample *Next;
} Sample;

typedef struct Meter
{
	char *ID;
	BYTE NodeAddress;
	enum CommandType
	{
		CommandType_ReadRegister = 0x03
	} Command;
	int StartAddress;
	int RegisterCount;

	int SampleCount;							// 采样点总数
	Sample *Samples;							// 采样点描述数组

	struct Meter *Prev;
	struct Meter *Next;

	Package PackageBuffer;
} Meter;

/* 数据查询 */
typedef struct SearchDefine
{
	char *DepartmentID;
	char *SwitchgearID;

	Meter *Meters;
} SearchDefine;

extern void PackageTxBuffer(Meter *mt);
extern void PackageAllTxBuffer(const SearchDefine *sd);
extern void AddSample(Meter *mt, Sample *sm);
extern void AddMeter(SearchDefine *sd, Meter *mt);
extern int ReadSearchDefine(const char *filepath, SearchDefine **sd);


#endif /* SEARCHDEFINE_H_ */
