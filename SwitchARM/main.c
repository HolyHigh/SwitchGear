/*
 * main.c
 *
 *  Created on: 2014年3月22日
 *      Author: Administrator
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "Units/cJSON.h"
#include "Drivers/em9280_GPIO.h"
#include "Global.h"
#include "Network.h"
#include "SearchDefine.h"
#include "Search.h"

int CodeTest(int codeid)
{
	if (codeid == 1)
	{
		/* test SearchDefine.json */
		//ReadSearchDefine("SearchDefine.json", Search);
		//printf("\n\n");
	} else if (codeid == 2)
	{
		/* test DeviceDefine.json */
		//ReadDeviceDefine("DeviceDefine.json", Device);
		//printf("\n\n");
	} else if (codeid == 3)
	{
		// GPIO test
		GPIO_Init(0xFFFFFFFF);
		BOOL bState = FALSE;
		while (TRUE)
		{
			bState = !bState;
			printf("\nSet GPIO 0 state: %d\tGPIO 1 state: %d", bState, !bState);
			GPIO_Out((1 << 0), bState);
			GPIO_Out((1 << 1), bState);
			sleep(5);
		}
		GPIO_Close();
	} else if (codeid == 4)
	{
		// alive test
		SendData(CreateAlivePacket(Search->DepartmentID, Search->SwitchgearID, GC.AliveMessage));
	}

	return 0;
}

int main(int argc, char ** argv)
{
	if (argc >= 2)
	{
		pthread_t network_thread;

		printf("\nCode testing...");

		InitGlobalConfig(&GC);
		Init();
		InitSearch();
		GPIO_Init(0xFFFFFFFF);

		pthread_create(&network_thread, NULL, thread_network, NULL);
		CodeTest(atoi(argv[1]));

		GPIO_Close();
	} else
	{
		pthread_t network_thread;

		printf("\n电力电压和开关状态上传");

		InitGlobalConfig(&GC);
		Init();
		InitSearch();

		GPIO_Init(0xFFFFFFFF);

		pthread_create(&network_thread, NULL, thread_network, NULL);

		thread_CommonSearch(Search);

		GPIO_Close();
	}

	return 0;
}


