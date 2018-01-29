
#include <avr/sleep.h>
#include "sleepmode.h"
#include <util/delay.h>


void powerDown(void)
{
	delay(100);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	sleep_disable();
}
/*
void Energy::Idle(void)
{
  delay(100);
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleeping = true;
	sleep_enable();
	sleep_mode();
	sleep_disable();
}

void Energy::SleepADC(void)
{
  delay(100);
	set_sleep_mode(SLEEP_MODE_ADC);
  sleeping = true;
	sleep_enable();
	sleep_mode();
	sleep_disable();
}

void Energy::PowerSave(void)
{
  delay(100);
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleeping = true;
	sleep_enable();
	sleep_mode();
	sleep_disable();
}

void standby(void)
{
	delay(100);
	set_sleep_mode(SLEEP_MODE_STANDBY);
	sleeping = true;
	sleep_enable();
	sleep_mode();
	sleep_disable();
}*/
