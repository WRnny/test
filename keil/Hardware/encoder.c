#include "encoder.h"




////GPIOA的中断函数
//void GROUP0_IRQHandler(void)
//{
//	uint32_t gpio_status;
//	
//	// 获取中断信号情况
//	
//	gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIOA, DL_GPIO_PIN_12);
//	
//	// 右侧编码器A相上升沿触发
//	if((gpio_status & DL_GPIO_PIN_12) == DL_GPIO_PIN_12)
//	{
//		//A相上升沿触发时检测B电平，B相为低电平
//		if(!DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_6))
//		{
//			encoder_Rmotor.temp_count--;
//		}else{
//			encoder_Rmotor.temp_count++;
//		}
//		
//	}
//	
//	//清除中断状态，清除所有A,B相的中断标志，防止重复进入
//	DL_GPIO_clearInterruptStatus(GPIOA, DL_GPIO_PIN_12 | DL_GPIO_PIN_6);
//}

////GPIOB的中断函数
//void GROUP1_IRQHandler(void)
//{	

//	uint32_t gpio_status;

//	// 获取中断信号情况
//		
//	gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIOB, DL_GPIO_PIN_23 | DL_GPIO_PIN_27);

//	// 左侧编码器A相上升沿触发
//	if((gpio_status & DL_GPIO_PIN_23) == DL_GPIO_PIN_23)
//	{
//		//A相上升沿触发时检测B电平，B相为低电平
//		if(!DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_27))
//		{
//			encoder_Lmotor.temp_count--;
//		}else{
//			encoder_Lmotor.temp_count++;
//		}
//	}
//	
//	//清除中断状态，清除所有A,B相的中断标志，防止重复进入
//	DL_GPIO_clearInterruptStatus(GPIOB, DL_GPIO_PIN_23 | DL_GPIO_PIN_27);
//}


//ENCODER_RES encoder_Rmotor;
//ENCODER_RES encoder_Lmotor;

////编码器初始化

//void encoder_init(void)
//{
//	//使能编码器引脚的外部中断
//	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);//清除中断挂起标志
//	NVIC_EnableIRQ(GPIOB_INT_IRQn);//使能中断
//	NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);
//	NVIC_EnableIRQ(GPIOA_INT_IRQn);
//}

////获取编码器计数值
//int	get_encoder_Rcount(void)
//{
//	return encoder_Rmotor.count;
//}

//int	get_encoder_Lcount(void)
//{
//	return encoder_Lmotor.count;
//}

////获取轮子的转向
//ENCODER_DIR get_encoder_Rdir(void)
//{
//	return encoder_Rmotor.dir;
//}

//ENCODER_DIR get_encoder_Ldir(void)
//{
//	return encoder_Lmotor.dir;
//}

////用来更新存储编码器数据
//void encoder_updat(void)
//{
//	encoder_Rmotor.count = encoder_Rmotor.temp_count;
//	encoder_Lmotor.count = encoder_Lmotor.temp_count;
//	
//	encoder_Rmotor.dir = (encoder_Rmotor.count > 0) ? FORWARD :
//						 (encoder_Rmotor.count < 0) ? REVERSAL : STATIONARY;
//	encoder_Lmotor.dir = (encoder_Lmotor.count > 0) ? FORWARD :
//						 (encoder_Lmotor.count < 0) ? REVERSAL : STATIONARY;
//	
//	//关闭中断，防止中断打断初始化
//	__disable_irq(); //AI神力不清楚可不可行
//	encoder_Rmotor.temp_count = 0;
//	encoder_Lmotor.temp_count = 0;	
//	__enable_irq();
//	//打开中断
//}
