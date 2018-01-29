/*
Enerlib: easy-to-use wrapper for AVR's Sleep lib.
By E.P.G. - 12/2012 - Ver. 1.0.1
*/

#ifndef _SLEEPMODE_
#define _SLEEPMODE_

#include "common.h"
void PowerDown(void);
void Idle(void);
void SleepADC(void);
void PowerSave(void);
void Standby(void);
BOOL WasSleeping(void);
};

#endif
