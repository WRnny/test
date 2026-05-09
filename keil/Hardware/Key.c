#include "Key.h"

uint8_t Key_Getnum()
{	
	uint8_t key_num=0;
	
	if(DL_GPIO_readPins(Key_Up_PORT,Key_Up_PIN)==0)
	{
		key_num=1;
		delay_cycles(10000000);
		if(DL_GPIO_readPins(Key_Up_PORT,Key_Up_PIN)==0)
			key_num=10;
	}
	if(DL_GPIO_readPins(Key_Left_PORT,Key_Left_PIN)==0)
	{
		key_num=2;
	}
	if(DL_GPIO_readPins(Key_Down_PORT,Key_Down_PIN)==0)
	{
		key_num=3;
	}
	if(DL_GPIO_readPins(Key_Right_PORT,Key_Right_PIN)==0)
	{
		key_num=4;
	}
	if(DL_GPIO_readPins(Key_Central_PORT,Key_Central_PIN)==0)
	{
		key_num=5;
	}
	return key_num;
}
//#define Key_Up_PORT                                                      (GPIOA)
//#define Key_Up_PIN                                              (DL_GPIO_PIN_17)
//#define Key_Up_IOMUX                                             (IOMUX_PINCM39)
///* Defines for Left: GPIOA.15 with pinCMx 37 on package pin 8 */
//#define Key_Left_PORT                                                    (GPIOA)
//#define Key_Left_PIN                                            (DL_GPIO_PIN_15)
//#define Key_Left_IOMUX                                           (IOMUX_PINCM37)
///* Defines for Right: GPIOA.14 with pinCMx 36 on package pin 7 */
//#define Key_Right_PORT                                                   (GPIOA)
//#define Key_Right_PIN                                           (DL_GPIO_PIN_14)
//#define Key_Right_IOMUX                                          (IOMUX_PINCM36)
///* Defines for Down: GPIOB.24 with pinCMx 52 on package pin 23 */
//#define Key_Down_PORT                                                    (GPIOB)
//#define Key_Down_PIN                                            (DL_GPIO_PIN_24)
//#define Key_Down_IOMUX                                           (IOMUX_PINCM52)
///* Defines for Central: GPIOB.25 with pinCMx 56 on package pin 27 */
//#define Key_Central_PORT                                                 (GPIOB)
//#define Key_Central_PIN                                         (DL_GPIO_PIN_25)
//#define Key_Central_IOMUX                                        (IOMUX_PINCM56)
