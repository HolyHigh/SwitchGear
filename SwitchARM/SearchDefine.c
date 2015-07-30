/*
 * SearchDefine.c
 *
 *  Created on: 2014年3月31日
 *      Author: Administrator
 */

#include <string.h>
#include "Global.h"
#include "Units/cJSON.h"

#include "SearchDefine.h"

void PackageTxBuffer(Meter *mt)
{
	if (mt->Command == CommandType_ReadRegister)
	{
		mt->PackageBuffer.TxLength = 8;
		mt->PackageBuffer.TxBuffer = malloc(mt->PackageBuffer.TxLength);
		mt->PackageBuffer.RxLength = 200;
		mt->PackageBuffer.RxBuffer = malloc(mt->PackageBuffer.RxLength);
		//mt->PackageBuffer.RxBufferValid = FALSE;

		mt->PackageBuffer.TxBuffer[0] = mt->NodeAddress;
		mt->PackageBuffer.TxBuffer[1] = mt->Command;
		mt->PackageBuffer.TxBuffer[2] = INT16HighByte(mt->StartAddress);
		mt->PackageBuffer.TxBuffer[3] = INT16LowByte(mt->StartAddress);
		mt->PackageBuffer.TxBuffer[4] = INT16HighByte(mt->RegisterCount);
		mt->PackageBuffer.TxBuffer[5] = INT16LowByte(mt->RegisterCount);
		INT16U crc16 = CRC16(mt->PackageBuffer.TxBuffer, 6);
		crc16 = crc_cal16(mt->PackageBuffer.TxBuffer, 6);
		mt->PackageBuffer.TxBuffer[6] = INT16LowByte(crc16);
		mt->PackageBuffer.TxBuffer[7] = INT16HighByte(crc16);
	}
}

void PackageAllTxBuffer(const SearchDefine *sd)
{
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	printf("\n>Packaging all Tx buffer...");

	if (sd && sd->Meters)
	{
		Meter *mt = sd->Meters;
		do
		{
			PackageTxBuffer(mt);

			printf("\n>Packaged: MeterID=%s, NodeAddress=0x%02X, TxBuffer: 0x", mt->ID, mt->NodeAddress);
			PrintBuffer(mt->PackageBuffer.TxBuffer, mt->PackageBuffer.TxLength);
		} while ((mt = mt->Next));
	}

	printf("\n>DONE");
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
}

void AddSample(Meter *mt, Sample *sm)
{
	if (mt && sm)
	{
		if (mt->Samples == NULL)
		{
			mt->Samples = sm;
			mt->Samples->Prev = NULL;
			mt->Samples->Next = NULL;
		} else
		{
			Sample *smTemp = mt->Samples;
			while (smTemp->Next)
			{
				smTemp = smTemp->Next;
			}
			smTemp->Next = sm;
			sm->Prev = smTemp;
			sm->Next = NULL;
		}
	}
}

void AddMeter(SearchDefine *sd, Meter *mt)
{
	if (sd && mt)
	{
		if (sd->Meters == NULL)
		{
			sd->Meters = mt;
			sd->Meters->Prev = NULL;
			sd->Meters->Next = NULL;
		} else
		{
			Meter *mtTemp = sd->Meters;
			while (mtTemp->Next)
			{
				mtTemp = mtTemp->Next;
			}
			mtTemp->Next = mt;
			mt->Prev = mtTemp;
			mt->Next = NULL;
		}
	}
}

void ShowSearchDefine(const SearchDefine *sd)
{
	if (sd)
	{
		printf("\n\nDepartmentID: %s", sd->DepartmentID);
		printf("\nSwitchgearID: %s", sd->SwitchgearID);

		if (sd->Meters)
		{
			Meter *mt = sd->Meters;
			do
			{
				printf("\nMeterID: %s, NodeAddress: 0x%02X, StartAddress: 0x%04X, RegisterCount: %d"
						, mt->ID, mt->NodeAddress, mt->StartAddress, mt->RegisterCount);

				if (mt->Samples)
				{
					Sample *sm = mt->Samples;
					do
					{
						printf("\nSample Name: %s, ValueType: %d"
							   , sm->Name, sm->ValueType);
						sm = sm->Next;
					} while (sm);
				}

				mt = mt->Next;
			} while (mt);
		}
	}
}

int ReadSearchDefine(const char *filepath, SearchDefine **sd)
{
	int res = 1, i, j;

	char * buf;
	cJSON * root = NULL, * item;

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	printf("\n>Reading %s...", filepath);

	if ((buf = get_file_content(filepath)) == NULL)
	{
		perror("\n>Cannot open SearchDefine json file!");
		return -1;
	}

	if ((root = cJSON_Parse(buf)) == NULL)
	{
		perror("\n>Cannot read SearchDefine json file!");
		return -1;
	}

	// check file format
	if ((item = cJSON_GetObjectItem(root, "Type")) == NULL)
	{
		perror("\n>Not SearchDefine json file!");
		return -1;
	}

	if (strcmp("SearchDefine", item->valuestring) != 0)
	{
		perror("\n>Not SearchDefine json file!");
		return -1;
	}

	// read department id
	if ((item = cJSON_GetObjectItem(root, "DepartmentID")) == NULL)
	{
		perror("\n>SearchDefine json file missing 'DepartmentID'!");
		return -1;
	}

	// TODO: 释放所有元素空间
	free(*sd);
	*sd = (SearchDefine *)malloc(sizeof(SearchDefine));
	(*sd)->DepartmentID = (char *)malloc(strlen(item->valuestring));
	strncpy((*sd)->DepartmentID, item->valuestring, strlen(item->valuestring));
	printf("\n>DepartmentID: %s", (*sd)->DepartmentID);

	// read switchgear id
	if ((item = cJSON_GetObjectItem(root, "SwitchgearID")) == NULL)
	{
		perror("\n>SearchDefine json file missing 'SwitchgearID'!");
		return -1;
	}

	(*sd)->SwitchgearID = (char *)malloc(strlen(item->valuestring));
	strncpy((*sd)->SwitchgearID, item->valuestring, strlen(item->valuestring));
	printf("\n>SwitchgearID: %s", (*sd)->SwitchgearID);

	// read meter array
	if ((item = cJSON_GetObjectItem(root, "Meters")) == NULL)
	{
		perror("\n>SearchDefine json file missing 'Meters'!");
		return -1;
	}

	if (item->type != cJSON_Array)
	{
		perror("\n>SearchDefine json file 'Meters' is not an array!");
		return -1;
	}

	int num = cJSON_GetArraySize(item);
	cJSON *meterItem, *meterItemTemp;

	for (i = 0; i < num; i++)
	{
		Meter *mt = (Meter *)malloc(sizeof(Meter));

		meterItem = cJSON_GetArrayItem(item, i);

		// read meter id
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "ID")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'ID'!", i);
			continue;
		}

		mt->ID = (char *)malloc(strlen(meterItemTemp->valuestring));
		strncpy(mt->ID, meterItemTemp->valuestring, strlen(meterItemTemp->valuestring));
		printf("\n>  Meter NO: %d 'ID': %s", i, mt->ID);

		// read meter node address
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "NodeAddress")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'NodeAddress'!", i);
			continue;
		}

		mt->NodeAddress = meterItemTemp->valueint;
		printf("\n>  Meter NO: %d 'NodeAddress': %d", i, mt->NodeAddress);

		// read meter command
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "Command")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'Command'!", i);
			continue;
		}

		mt->Command = meterItemTemp->valueint;
		printf("\n>  Meter NO: %d 'Command': %d", i, mt->Command);

		// read meter start address
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "StartAddress")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'StartAddress'!", i);
			continue;
		}

		mt->StartAddress = strtol(meterItemTemp->valuestring, NULL, 16);
		printf("\n>  Meter NO: %d 'StartAddress': 0x%04X", i, mt->StartAddress);

		// read meter register count
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "RegisterCount")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'RegisterCount'!", i);
			continue;
		}

		mt->RegisterCount = meterItemTemp->valueint;
		printf("\n>  Meter NO: %d 'RegisterCount': %d", i, mt->RegisterCount);

		///////////////////////////////////////////////////////////////////////////////////////////
		// read meter array
		if ((meterItemTemp = cJSON_GetObjectItem(meterItem, "Samples")) == NULL)
		{
			printf("\n>  SearchDefine json file Meter NO: %d missing 'Samples'!", i);
			continue;
		}

		if (meterItemTemp->type != cJSON_Array)
		{
			printf("\n>  SearchDefine json file Meter NO: %d 'Samples' is not an array!", i);
			continue;
		}

		int sampleNum = cJSON_GetArraySize(meterItemTemp);
		cJSON *sampleItem, *sampleItemTemp;

		for (j = 0; j < sampleNum; j++)
		{
			Sample *sm = (Sample *)malloc(sizeof(Sample));

			// read sample array
			sampleItem = cJSON_GetArrayItem(meterItemTemp, j);

			// read sample name
			if ((sampleItemTemp = cJSON_GetObjectItem(sampleItem, "Name")) == NULL)
			{
				printf("\n>    SearchDefine json file Meter NO: %d Sample NO: %d missing 'Name'!", i, j);
				continue;
			}

			sm->Name = (char *)malloc(strlen(sampleItemTemp->valuestring));
			strncpy(sm->Name, sampleItemTemp->valuestring, strlen(sampleItemTemp->valuestring));
			printf("\n>    Meter NO: %d Sample NO: %d 'Name': %s", i, j, sm->Name);

			// read sample value type
			if ((sampleItemTemp = cJSON_GetObjectItem(sampleItem, "ValueType")) == NULL)
			{
				printf("\n>    SearchDefine json file Meter NO: %d Sample NO: %d missing 'ValueType'!", i, j);
				continue;
			}

			if (strcmp("Float", sampleItemTemp->valuestring) == 0)
			{
				sm->ValueType = SampleValueType_Float;
			} else if (strcmp("Bit", sampleItemTemp->valuestring) == 0)
			{
				sm->ValueType = SampleValueType_Bit;
			} else
			{
				sm->ValueType = SampleValueType_Unknown;
			}
			printf("\n>    Meter NO: %d Sample NO: %d 'ValueType': %d", i, j, sm->ValueType);

			AddSample(mt, sm);
		}
		///////////////////////////////////////////////////////////////////////////////////////////

		AddMeter((*sd), mt);
	}

	//ShowSearchDefine((*sd));

	free(buf);
	cJSON_Delete(root);

	printf("\n>Reading %s completed!", filepath);
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	fflush(stdout);

	return res;
}
