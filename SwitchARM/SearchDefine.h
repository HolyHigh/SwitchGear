/*
 * SearchDefine.h
 *
 *  Created on: 2014��3��31��
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
	char *Name;									// ���������ƣ������ϴ�ʱָ���������ƣ�
	SampleValueType ValueType;					// ������ֵ����
	float FloatValue;							// �洢�������͵�ֵ
	int IntValue;								// �洢�������͵�ֵ

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

	int SampleCount;							// ����������
	Sample *Samples;							// ��������������

	struct Meter *Prev;
	struct Meter *Next;

	Package PackageBuffer;
} Meter;

/* ���ݲ�ѯ */
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
