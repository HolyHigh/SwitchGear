/*
 * em9280_GPIO.c
 *
 *  Created on: 2014年4月20日
 *      Author: Administrator
 */

#include <fcntl.h>
#include <unistd.h>

#include "../Global.h"
#include "em9280_drivers.h"
#include "em9280_GPIO.h"

/*
 * em9280 GPIO 操作对象
 */
int GPIOFD = 0;

/*
 *
 * dwEnBits : 位为 1 时，表示该位为 输出
 *            0xFFFFFFFF设定所有 GPIO 口为输出
 */
BOOL GPIO_Init(DWORD dwEnBits)
{
	int rc, i;

	if ((GPIOFD = open("/dev/em9280_gpio", O_RDWR)) == -1)
	{
		perror("\nFail to open em9280 GPIO!");
		return FALSE;
	}

	if((rc = GPIO_OutEnable(GPIOFD, dwEnBits)) < 0)	// set GPIO as output
	{
		printf("GPIO_OutEnable::failed %d\n", rc);
		return FALSE;
	}

	for(i = 0; i < 32; i++)
	{
		rc = GPIO_OutClear(GPIOFD, (1 << i));
	}

	return TRUE;
}

void GPIO_Close()
{
	close(GPIOFD);
}

void GPIO_Out(DWORD dwBits, BOOL bOn)
{
	int rc;

	if (GPIOFD)
	{
		if (bOn)
		{
			rc = GPIO_OutSet(GPIOFD, dwBits);
		} else
		{
			rc = GPIO_OutClear(GPIOFD, dwBits);
		}

		if(rc < 0)
		{
			printf("GPIO_Out::failed %d\n", rc);
		}
	} else
	{
		perror("\nem9280 GPIO not opened!");
	}
}

int GPIO_OutEnable(int fd, unsigned int dwEnBits)
{
	int 				rc;
	struct double_pars	dpars;

	dpars.par1 = EM9280_GPIO_OUTPUT_ENABLE;		// 0
	dpars.par2 = dwEnBits;

	rc = write(fd, &dpars, sizeof(struct double_pars));
	return rc;
}

int GPIO_OutDisable(int fd, unsigned int dwDisBits)
{
	int 				rc;
	struct double_pars	dpars;

	dpars.par1 = EM9280_GPIO_OUTPUT_DISABLE;	// 1
	dpars.par2 = dwDisBits;

	rc = write(fd, &dpars, sizeof(struct double_pars));
	return rc;
}

int GPIO_OutSet(int fd, unsigned int dwSetBits)
{
	int 				rc;
	struct double_pars	dpars;

	dpars.par1 = EM9280_GPIO_OUTPUT_SET;	// 2
	dpars.par2 = dwSetBits;

	rc = write(fd, &dpars, sizeof(struct double_pars));
	return rc;
}

int GPIO_OutClear(int fd, unsigned int dwClearBits)
{
	int 				rc;
	struct double_pars	dpars;

	dpars.par1 = EM9280_GPIO_OUTPUT_CLEAR;	// 3
	dpars.par2 = dwClearBits;

	rc = write(fd, &dpars, sizeof(struct double_pars));
	return rc;
}

int GPIO_PinState(int fd, unsigned int* pPinState)
{
	int 				rc;
	struct double_pars	dpars;

	dpars.par1 = EM9280_GPIO_INPUT_STATE;	// 5
	dpars.par2 = *pPinState;

	rc = read(fd, &dpars, sizeof(struct double_pars));
	if(!rc)
	{
		*pPinState = dpars.par2;
	}
	return rc;
}
