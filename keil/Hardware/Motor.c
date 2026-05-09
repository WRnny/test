#include "Motor.h"
#include "../custom_definitions.h"

#define speed_min 0
#define speed_max 9999

void Motor_Init()
{
	DL_TimerA_startCounter(MotorB_INST);
	DL_TimerA_startCounter(MotorA_INST);
}

void SetR_duty(int speed)
{
	
	int R_speed;
	
	if(speed >= 0)
	{
		speed = fminf(speed, speed_max);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_16);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_15);
		R_speed = 9999 - speed;
		DL_TimerG_setCaptureCompareValue(TIMG7,R_speed, DL_TIMER_CC_1_INDEX);
	}
	else
	{
		speed = -fmaxf(speed, -speed_max);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_16);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_15);
		R_speed = 9999 - speed;
		DL_TimerG_setCaptureCompareValue(TIMG7,R_speed, DL_TIMER_CC_1_INDEX);
	}
	
}

void SetL_duty(int speed)
{
	
	int L_speed;
	
	if(speed >= 0)
	{
		speed = fminf(speed, speed_max);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_2);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_3);
		L_speed = 9999 - speed;
		DL_TimerG_setCaptureCompareValue(TIMA1,L_speed, DL_TIMER_CC_1_INDEX);
	}
	else
	{
		speed = -fmaxf(speed, -speed_max);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_2);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_3);
		L_speed = 9999 - speed;
		DL_TimerG_setCaptureCompareValue(TIMA1,L_speed, DL_TIMER_CC_1_INDEX);
	}
}

void SetR_stop(void)
{
	DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_16);
	DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_15);

}
