#include "delay.h"

uint32_t delay_times;

void Delay_ms(volatile uint32_t ms)
{
	delay_times = ms;
	while(delay_times != 0);
}

void SysTick_Handler(void)
{
	if(delay_times != 0)
	{
		delay_times--;
	}
}
