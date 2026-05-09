#include "Alert.h"

void Led1_ON()
{
	DL_GPIO_setPins(GPIO_LED_PORT,GPIO_LED_Led1_PIN);
}
void Led1_OFF()
{
	DL_GPIO_clearPins(GPIO_LED_PORT,GPIO_LED_Led1_PIN);
}
void Led2_ON()
{
	DL_GPIO_setPins(GPIO_LED_PORT,GPIO_LED_Led2_PIN);
}
void Led2_OFF()
{
	DL_GPIO_clearPins(GPIO_LED_PORT,GPIO_LED_Led2_PIN);
}
void Led1_turn()
{
	Led1_ON();
	delay_cycles(500000);
	Led1_OFF();
}
void Led2_turn()
{
	Led2_ON();
	delay_cycles(500000);
	Led2_OFF();
}
void Buzzer_OFF()
{
	DL_TimerG_setCaptureCompareValue(Buzzer_INST,5000,DL_TIMER_CC_0_INDEX);
}
void Buzzer_ON()
{
	DL_TimerG_setCaptureCompareValue(Buzzer_INST,200,DL_TIMER_CC_0_INDEX);
}
void Buzzer_turn()
{
	Buzzer_ON();
	delay_cycles(500000);
	Buzzer_OFF();
}