/*
 * em9280_GPIO.h
 *
 *  Created on: 2014Äê4ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef EM9280_GPIO_H_
#define EM9280_GPIO_H_

#include "../Global.h"

extern BOOL GPIO_Init(DWORD dwEnBits);
extern void GPIO_Close();
extern void GPIO_Out(DWORD dwBits, BOOL bOn);
extern int GPIO_OutEnable(int fd, unsigned int dwEnBits);
extern int GPIO_OutDisable(int fd, unsigned int dwDisBits);
extern int GPIO_OpenDrainEnable(int fd, unsigned int dwODBits);
extern int GPIO_OutSet(int fd, unsigned int dwSetBits);
extern int GPIO_OutClear(int fd, unsigned int dwClearBits);
extern int GPIO_PinState(int fd, unsigned int* pPinState);

#endif /* EM9280_GPIO_H_ */
