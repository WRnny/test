#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "keil/Hardware/Motor.h"
#include "keil/Hardware/Track.h"
#include "keil/Hardware/Serial.h"
#include "keil/Algorithm/PID.h"
#include "keil/Hardware/delay.h"
#include "keil/Hardware/Alert.h"

#define ABS(x) ( (x) < 0 ? -(x) : (x) )


static char uart0_rx_buf[128];//接收缓存区
static uint8_t uart0_rx_index = 0;//接收索引
float Yaw_received = 0.0f;//Yaw轴接收数据
uint8_t Yaw_RxFlag = 0;//Yaw轴接收数据索引
float Yaw_sum = 0;//初始化Yaw轴数据积累
float Basic_set_yaw = 0;//上电测的基准轴

extern volatile float coord;//红外循迹监测位置

volatile int encoder_r_count = 0;//右侧编码器测量值
volatile int encoder_l_count = 0;//左侧编码器测量值
volatile int encoder_r_sum = 0;//右侧编码器计数总值 --> 用于估计路程
volatile int encoder_l_sum = 0;//左侧侧编码器计数总值 --> 用于估计路程
volatile float motor_Lmeasure;//左侧电机实际值
volatile float motor_Rmeasure;//右侧电机实际值
volatile int measure_r_count = 0;//编码器计算右侧电机速度
volatile int measure_l_count = 0;//编码器计算左侧电机速度

volatile int motor_Loutput = 0;  // 左侧电机输出		max --> 2300
volatile int motor_Routput = 0;  // 右侧电机输出		max --> 2200

volatile float Track = 0.0f;//循迹PID输出值

volatile float yaw_output = 0.0f;//yaw轴PID输出值

int Basic_set_speed = 500;//电机基础速度	500

float set_speed_L = 0;  // 左侧电机目标速度
float set_speed_R = 0;  // 右侧电机目标速度

float set_track = 4.0f;//循迹PID目标值

float set_yaw = 0.0f;//yaw轴PID目标值

float error_L = 0.0f, error_L_last = 0.0f, integral_L = 0.0f, derivative_L = 0.0f;//左轮PID变量
float error_R = 0.0f, error_R_last = 0.0f, integral_R = 0.0f, derivative_R = 0.0f;//右轮PID变量

float error_Track = 0.0f, error_Track_last = 0.0f, integral_Track = 0.0f, derivative_Track = 0.0f;//循迹PID变量

float error_yaw = 0.0f, error_yaw_last = 0.0f, integral_yaw = 0.0f, derivative_yaw = 0.0f;//yaw轴PID变量

#define KP_L 7.0f    // 比例系数7.0
#define KI_L 0.8f    // 积分系数0.8
#define KD_L 1.4f   // 微分系数1.4

#define KP_R 10.0f
#define KI_R 1.35f
#define KD_R 2.5f//2.5

#define KP_Track 155.0f//155
#define KI_Track 0.0f//0
#define KD_Track 2.0f//2

volatile float KP_yaw = 10.0f;//30
#define KI_yaw 0.0f
#define KD_yaw 2.0f

float array[20];

int count = 0;//不同赛题通用的计数装置
int Flag_mode = 0;//按键状态用于调整赛题模式
int Flag_yawmode = 0;//需要用到yaw轴的标志位
int Flag_A2 = 0;//A2赛题进入二阶段标志位
int Flag_A3 = 0;// A3赛题进入二阶段标志位
int Flag_A4 = 0;
int time_count = 0;//A2赛题防阻塞计时变量
int times_yaw = 1;//用于获取初始yaw轴数据用于定向
int Flag_A5 = 0;//A5赛题进入第二阶段标志位
int Flag_A6set = 0;//进入A6赛题设置标志位
int Flag_A6 = 0;//A6赛题标志位
int timer_measure = 0;//用于区分按键Central的长按和短按
int laps = 0;//记录A6赛题的绕圈数
int time_a6 =0;//调整角度环计数，角度环只调整一次
int time_A6 =0;//调整角度环计数，角度环只调整一次
extern volatile int Flag_coord;//红外循迹模块未检测到黑线标志位

extern int pin_number[7];
extern volatile float qty;

int main(void)
{
	SYSCFG_DL_init();
		
	DL_TimerA_startCounter(TIMG8);//打开无源蜂鸣器PWM通道
	
	Motor_Init();//电机初始化
	
	NVIC_ClearPendingIRQ(UART0_INT_IRQn);//清除UART0中断 --> 中断用于调试
	
	NVIC_EnableIRQ(UART0_INT_IRQn);//使能UART0中断 --> 串口中断用于调试
	
	NVIC_ClearPendingIRQ(UART1_INT_IRQn);//清除yaw轴获取中断标志位
	
	NVIC_EnableIRQ(UART1_INT_IRQn);//使能yaw轴获取中断
		
	NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);//清除TIMER1定时中断标志
	
	NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);//使能TIMER1定时中断
	
	DL_TimerG_startCounter(TIMER_1_INST);//启动定时器
	
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);//清楚编码器中断标志
	
	NVIC_EnableIRQ(GPIOB_INT_IRQn);//使能编码器中断
	
	NVIC_ClearPendingIRQ(TIMG0_INT_IRQn);//清除测速定时中断标志位
	
	NVIC_EnableIRQ(TIMG0_INT_IRQn);//使能测速定时中断
		
	while(1)
	{			
		track();//红外循迹检测放在这循迹更稳定
		//开机上电时获取五十次yaw轴数据的平均值作为基准轴
		if(Yaw_RxFlag && times_yaw <= 50)
		{
			Yaw_sum += Yaw_received;
			if(times_yaw == 50)
			{
				Basic_set_yaw = Yaw_sum / (times_yaw * 1.0);
			}
			times_yaw++;
			Yaw_RxFlag = 0;
		}
			
		//按键UP执行A1题程序
		if(!Flag_A6set && DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_17) == 0 )
		{
			Delay_ms(20);
			while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_17) == 0);
			Delay_ms(20);
			
			Flag_mode = 1;//进入A1题程序标志
		}

		//A1题B点停车条件判断
		if(Flag_mode == 1 && encoder_r_sum > 1200 && encoder_l_sum > 1200 && Flag_coord)//待实际测量
		{
			Flag_mode = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Delay_ms(500);
			
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Buzzer_ON();
			Delay_ms(500);
			Buzzer_OFF();
		}



		//按键DOWN执行A2题程序
		if(!Flag_A6set && DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_24) == 0)
		{
			Delay_ms(20);
			while( DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_24) == 0);
			Delay_ms(20);
			
			Flag_mode = 2;//进入A2题程序标志
			
		}
		
		//A2题过第一个直角开启角度环
		if(Flag_mode == 2 && Flag_A2 == 0 && encoder_r_sum > 900 && encoder_l_sum > 900)//待实际测量
		{	
			Flag_mode = 0;//防止重复进入中断
						
			encoder_r_sum = 0;
			encoder_l_sum = 0;
						
			Flag_mode = 2;
			Flag_A2 = 1;
		}
		//A2题抖动路段角度环定位并且痛过干扰路段
		if(Flag_mode == 2 && Flag_A2 == 1)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw += 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
			}
			if(encoder_r_sum >1000 && encoder_l_sum > 1000)
			{
				Flag_A2 = 3;
				Flag_mode = 2;
			}
			
		}
		//A2过直角判断
		if(Flag_A2 == 3 && Flag_mode == 2 && ABS(Yaw_received - Basic_set_yaw) > 80)
		{
			Flag_mode = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Delay_ms(500);
			
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Buzzer_ON();
			Delay_ms(500);
			Buzzer_OFF();

		}
		
		//按键Right执行A3题程序
		if(!Flag_A6set && DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_14) == 0)
		{
			Delay_ms(20);
			while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_14) == 0);
			Delay_ms(20);
			
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Flag_mode = 3;
		}
		
		//A3题过第一个直角开启角度环
		if(Flag_mode == 3 && Flag_A3 == 0 &&  encoder_r_sum > 900 && encoder_l_sum > 900)//待实际测量
		{
			Flag_mode = 0;//防止重复进入中断
						
			encoder_r_sum = 0;
			encoder_l_sum = 0;
						
			Flag_mode = 3;
			Flag_A3 = 1;
		}
		//A3题抖动路段角度环定位并且痛过干扰路段
		if(Flag_mode == 3 && Flag_A3 == 1)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw += 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
			}
			if(encoder_r_sum >1000 && encoder_l_sum > 1000)
			{
				Flag_A3 = 2;
				Flag_mode = 3;
					
				time_A6 = 0;
			}

		}
		// 经过干扰路段，开启循迹模式
		if(Flag_mode == 3 && Flag_A3 == 2 && Flag_coord > 100 && encoder_r_sum >2000 && encoder_l_sum > 2000)
		{
			
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Flag_A3 =3;

		}
		// 开启角度环定位
		if(Flag_mode == 3 && Flag_A3 == 3)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw -= 180.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
			}

			if(Flag_coord == 0)
			{
				Flag_A3 = 4;
				Flag_mode = 3;

			}
		}
		// 停车判断
		if(Flag_mode == 3 && Flag_A3 == 4 && Flag_Black == 1)
		{
			Flag_mode = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Delay_ms(500);
			
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Buzzer_ON();
			Delay_ms(500);
			Buzzer_OFF();

		}


		//按键Left执行A4题程序
		if(!Flag_A6set && DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_15) == 0)
		{
			Delay_ms(20);
			while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_15) == 0);
			Delay_ms(20);

			Flag_mode = 4;//进入A4题程序标志
		}
		
		//A4题过第一个直角开启角度环
		if(Flag_mode == 4 && Flag_A4 == 0 &&  encoder_r_sum > 900 && encoder_l_sum > 900)//待实际测量
		{
			Flag_mode = 0;//防止重复进入中断
						
			encoder_r_sum = 0;
			encoder_l_sum = 0;
						
			Flag_mode = 4;
			Flag_A4 = 1;
		}
		//A4题抖动路段角度环定位并且痛过干扰路段
		if(Flag_mode == 4 && Flag_A4 == 1)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw += 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
			}
			if(encoder_r_sum >1000 && encoder_l_sum > 1000)
			{
				Flag_A4 = 2;
				Flag_mode = 4;
					
				time_A6 = 0;
			}

		}
		// 经过干扰路段，开启循迹模式
		if(Flag_mode == 4 && Flag_A4 == 2 && encoder_r_sum >2000 && encoder_l_sum > 2000)
		{
			
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Flag_A4 =3; // 进入环岛循迹状态
		}
		
		if(Flag_mode == 4 && Flag_A4 == 3 && encoder_r_sum >350 && encoder_l_sum > 350 && Flag_RigjtAngle == 1)
		{
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Flag_A4 = 4;
		}
		
		if(Flag_mode == 4 && Flag_A4 == 4)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw -= 180.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
				
				Flag_A4 = 5;
				time_A6 = 0;
			}
		}
		
		if(Flag_mode == 4 && Flag_A4 == 5 && encoder_r_sum >1000 && encoder_l_sum > 1000 && Flag_Black == 1)
		{
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Flag_A4 = 6;

		}
		
		if(Flag_mode == 4 && Flag_A4 == 6)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw -= 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 10 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
				
				Flag_A4 = 7;
				time_A6 = 0;
			}
		}
		if(Flag_mode == 4 && Flag_A4 == 7 && Flag_coord > 500)
		{
			if(!time_A6) // A6是之前赛题用的标志位，为了不乱暂时先不搞其他标志位
			{
				Basic_set_yaw -= 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1 && time_A6 == 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				time_A6++;
			}

			if(Flag_coord == 0)
			{
				Flag_A4 = 8;
				Flag_mode = 4;
			}

		}
		
		if(Flag_mode == 4 && Flag_A4 == 8 && Flag_Black == 1)
		{
			Flag_mode = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Delay_ms(500);
			
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Buzzer_ON();
			Delay_ms(500);
			Buzzer_OFF();

		}

			
		

		
		
		//A4题C点停车条件判断
//		if(Flag_mode == 4 && encoder_r_sum > 2300 && encoder_l_sum > 2300)//待实际测量
//		{
//			Flag_mode = 0;
//			
//			//变量初始化
//			set_speed_L = 0;
//			set_speed_R = 0;
//			coord = 4.0f;
//			encoder_r_sum = 0;
//			encoder_l_sum = 0;
//			
//			Delay_ms(500);
//			
//			//声光提示部分
//			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
//			
//			Buzzer_ON();
//			Delay_ms(500);
//			Buzzer_OFF();
//		}

		//短按按键按键Central执行A5题程序,长按按键按键Central执行A6题程序
		if(!Flag_A6set && DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_25) == 0)
		{
			Delay_ms(20);
			//区分短按长按简单逻辑
			while( DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_25) == 0)
			{
				Delay_ms(10);
				timer_measure++;
				if(timer_measure >= 50)
				{
					Flag_A6set = 1;
				}
			}
			Delay_ms(20);
			
			if(timer_measure < 50)
			{
				Flag_yawmode = 2;//进入A5题程序标志
			}
			
			}
		
		//A5题F点声光处理加判断
		if(Flag_yawmode == 2 && Flag_A5 == 0  && encoder_r_sum > 560 && encoder_l_sum > 560)
		{
			Flag_yawmode = 0;//防止重复进入中断
			
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			//变量初始化
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Buzzer_ON();
			
			Flag_yawmode = 2;
			Flag_A5 = 1;
		}
		//A5题F点利用角度环顺时针旋转正对E点
		if(Flag_yawmode == 2 && Flag_A5 == 1)
		{
			static int time =0;
			if(!time)
			{
				Basic_set_yaw += 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
			}
			if(ABS(Basic_set_yaw - Yaw_received) < 3)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				Flag_A5 = 3;//角度调整完毕进入断路循迹
			}
			time++;
		}
		//A5题E点停车条件判断
		if(Flag_yawmode == 2 && Flag_A5 == 3  && encoder_r_sum > 900 && encoder_l_sum > 900)
		{
			Flag_yawmode = 0;//防止重复进入中断
			Flag_A5 = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Delay_ms(500);
			//声光提示部分
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);	
			Buzzer_ON();
			Delay_ms(500);
			Buzzer_OFF();
		}

		//按键设置A6赛题逻辑
		if(Flag_A6set == 1)
		{
			//进入A6赛题提示音
			if(!count)
			{
				Buzzer_ON();
				Delay_ms(100);
				Buzzer_OFF();
				count++;
			}
			//按键Left增加绕圈数
			if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_15) == 0)
			{
				Delay_ms(20);
				while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_15) == 0);
				Delay_ms(20);
				
				laps++;
				Buzzer_turn();
			}
			//按键Right减少绕圈数
			if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_14) == 0)
			{
				Delay_ms(20);
				while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_14) == 0);
				Delay_ms(20);
				if(laps)
				{
					laps--;
				}
				Buzzer_turn();
			}
			//按键UP检查当前绕圈数
			if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_17) == 0)
			{
				Delay_ms(20);
				while( DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_17) == 0 );
				Delay_ms(20);
				
				for(int i = 0; i < laps; i++)
				{
					DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
					Delay_ms(200);
					DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
					Delay_ms(200);
				}
			}
			//按键Down红外循迹调试模式
			if(DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_24) == 0)
			{
				Delay_ms(20);
				while( DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_24) == 0 );
				Delay_ms(20);
				
				Flag_mode = 66;//循迹调试模式
			}

			
			//短按按键Central开始执行A6赛题
			if(laps && DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_25) == 0)
			{
				//开始提示音
				Buzzer_ON();
				Delay_ms(100);
				Buzzer_OFF();

				Flag_A6 = 1;//开始A6赛题
				Flag_A6set = 2;//A6赛题设置完毕
			}

			
		}
		//A6赛题到达E点处理
		if(laps && Flag_A6set == 2 && Flag_A6 == 1 && encoder_r_sum > 3600 && encoder_l_sum > 3600 && Flag_coord > 500)
		{
			//防止重复进入中断
			Flag_A6set = 0;
			Flag_A6 = 0;
			
			//变量初始化
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			time_count = 0;
			
			Flag_A6set = 2;
			Flag_A6 = 2;
		}
		//A6赛题到E点开启角度环定位
		if(laps && Flag_A6set == 2 && Flag_A6 == 2)
		{
			if(!time_A6)
			{
				Basic_set_yaw += 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_A6++;
			}
			
			if(ABS(Basic_set_yaw - Yaw_received) < 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				Flag_A6 = 3;//角度调整完毕进入断路循迹
			}
			
		}
		//A6赛题在从E点到F点位置判断
		if(laps && Flag_A6set == 2 && Flag_A6 == 3 && encoder_r_sum > 850 && encoder_l_sum > 850)
		{
			//防止重复进入中断
			Flag_A6set = 0;
			Flag_A6 = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			Flag_A6set = 2;
			Flag_A6 = 4;
		}
		//A6赛题到F点开启角度环定位
		if(laps && Flag_A6set == 2 && Flag_A6 == 4)
		{
			
			if(!time_a6)
			{
				Basic_set_yaw -= 90.0f;
				// 角度归一化到-180~180范围
				while(Basic_set_yaw > 180.0f) Basic_set_yaw -= 360.0f;
				while(Basic_set_yaw < -180.0f) Basic_set_yaw += 360.0f;
				time_a6++;

			}
			if(ABS(Basic_set_yaw - Yaw_received) < 1)
			{
				encoder_r_sum = 0;
				encoder_l_sum = 0;
				Flag_A6 = 5;//角度调整完毕进入断路循迹
			}
		}
		//A6赛题从F点到A点到达判断
		if(laps && Flag_A6set == 2 && Flag_A6 == 5 && encoder_r_sum > 500 && encoder_l_sum > 500)
		{
			//防止重复进入中断
			Flag_A6set = 0;
			Flag_A6 = 0;
			time_a6 = 0;
			time_A6 = 0;
			
			//变量初始化
			set_speed_L = 0;
			set_speed_R = 0;
			coord = 4.0f;
			encoder_r_sum = 0;
			encoder_l_sum = 0;
			
			//减少圈数
			laps--;
			if(laps <= 0)
			{
				laps = 0;
			}
			//声光处理
			DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
			
			Buzzer_ON();
			
			if(laps)
			{
				Flag_A6set = 2;
				Flag_A6 = 1;
			}
		}

		//调试红外循迹长按Central,之后按按键down
		
	}
}
//红外循迹PID中断 + 断路循迹PID中断 --> 50ms
void TIMER_1_INST_IRQHandler(void)
{
  switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) //获取中断挂起标志
	{
      case DL_TIMER_IIDX_ZERO://零事件中断，向下计数模式，计数到0中断
			//循迹统一进入的PID调控
			if (Flag_mode && Flag_A4 != 6 && Flag_A4 != 4)
			{
//				track();
				
				error_Track = set_track - coord;
				integral_Track += error_Track;

				// 积分限幅
				if (integral_Track > 50.0f) integral_Track = 50.0f;
				if (integral_Track < -50.0f) integral_Track = -50.0f;

				derivative_Track = error_Track - error_Track_last;

				Track = KP_Track * error_Track + KI_Track * integral_Track + KD_Track * derivative_Track;

				error_Track_last = error_Track;

					// 输出限幅
//				if (Track > 300.0f) Track = 300.0f;
//				if (Track < -300.0f) Track = -300.0f;
					
				set_speed_L = Basic_set_speed - Track;//原来是-逻辑貌似反了测试
				set_speed_R = Basic_set_speed + Track;
			}

						
			//A2赛题防阻塞计时功能函数
			if( time_count <= 10 && (Flag_yawmode == 2 && Flag_A5 == 1) || (Flag_A6set == 2 && Flag_A6 == 1) )
			{
				DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_18);
				if(time_count > 5)
				{
					Buzzer_OFF();
				}
				time_count++;
			}

			
			//暂时计划是把每一个涉及到yaw轴的赛题单独搞一个PID处理
			if(Flag_A2 == 1 || Flag_A3 == 1 || Flag_A3 == 3 || Flag_A4 == 1 || Flag_A4 == 4 || Flag_A4 == 7)
			{
				error_yaw = Basic_set_yaw - Yaw_received;
				integral_yaw += error_yaw;

				// 积分限幅
				if (integral_yaw > 30.0f) integral_yaw = 30.0f;
				if (integral_yaw < -30.0f) integral_yaw = -30.0f;

				derivative_yaw = error_yaw - error_yaw_last;

				yaw_output = KP_yaw * error_yaw + KI_yaw * integral_yaw + KD_yaw * derivative_yaw;

				error_yaw_last = error_yaw;

					// 输出限幅
//				if (yaw_output > 500.0f) yaw_output = 500.0f;
//				if (yaw_output < -500.0f) yaw_output = -500.0f;
					
				set_speed_L = Basic_set_speed - yaw_output;
				set_speed_R = Basic_set_speed + yaw_output;

			}
			
			//A5赛题顺时针旋转而单独搞出来的角度环
			if( Flag_A4 == 6)
			{
				error_yaw = Basic_set_yaw - Yaw_received;
				integral_yaw += error_yaw;

				// 积分限幅
				if (integral_yaw > 30.0f) integral_yaw = 30.0f;
				if (integral_yaw < -30.0f) integral_yaw = -30.0f;

				derivative_yaw = error_yaw - error_yaw_last;

				yaw_output = 5.0f * error_yaw + KI_yaw * integral_yaw + KD_yaw * derivative_yaw;

				error_yaw_last = error_yaw;

					// 输出限幅
//				if (yaw_output > 200.0f) yaw_output = 200.0f;
//				if (yaw_output < -200.0f) yaw_output = -200.0f;
					
				set_speed_L = Basic_set_speed + yaw_output;
				set_speed_R = Basic_set_speed - yaw_output;
				
			}

			array[0] = encoder_r_sum;//Basic_set_yaw
			array[1] = encoder_l_sum;
			array[2] = Flag_A4;
			array[3] = Flag_RigjtAngle;
			array[4] = Yaw_received;
			array[5] = Basic_set_yaw;
//			array[6] = pin_number[5];
//			array[7] = pin_number[6];
//			array[8] = qty;
//			array[9] = Flag_coord;
			VOFA_SendData(array, 6);

			DL_TimerG_clearInterruptStatus(TIMER_1_INST, DL_TIMER_IIDX_ZERO);		
			
			break;
      default:
          break;
  }

}

//编码器计数中断
void GROUP1_IRQHandler(void)
{
  if(DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) 
	{

		// 检查左侧编码器B相中断
		if (DL_GPIO_getEnabledInterruptStatus(GPIOB, DL_GPIO_PIN_6))
		{
			if(DL_GPIO_readPins(GPIOA, DL_GPIO_PIN_12) == 0) 
			{
				encoder_l_count--;
			} 
			else {
				encoder_l_count++;
			}
						
			DL_GPIO_clearInterruptStatus(GPIOB, DL_GPIO_PIN_6);
			
		}
		
		// 检查右编码器B相中断
		if (DL_GPIO_getEnabledInterruptStatus(GPIOB, DL_GPIO_PIN_27)) // DL_GPIO_PIN_27
		{
			if(DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_23) == 0) 
			{
				encoder_r_count++;
			} else {
				encoder_r_count--;
			}
			
		

						
			DL_GPIO_clearInterruptStatus(GPIOB, DL_GPIO_PIN_27);
		}
	}
		
}

//电机PID中断 --> 20ms
void TIMG0_IRQHandler(void)
{
	if( DL_TimerG_getPendingInterrupt(TIMG0) == DL_TIMER_IIDX_ZERO )
	{
		encoder_r_sum += encoder_r_count;
		encoder_l_sum += encoder_l_count;
		
		measure_r_count = encoder_r_count;
		measure_l_count = encoder_l_count;
		
		motor_Lmeasure = measure_l_count / 0.05f;
		motor_Rmeasure = measure_r_count / 0.05f;
		

		error_L = set_speed_L - motor_Lmeasure;
		integral_L += error_L;

		// 积分限幅
		if (integral_L > 7000.0f) integral_L = 7000.0f;
		if (integral_L < -7000.0f) integral_L = -7000.0f;

		derivative_L = error_L - error_L_last;

		motor_Loutput = KP_L * error_L + KI_L * integral_L + KD_L * derivative_L;

		error_L_last = error_L;

			// 输出限幅
		if (motor_Loutput > 10000.0f) motor_Loutput = 10000.0f;
		if (motor_Loutput < -10000.0f) motor_Loutput = -10000.0f;
					
		SetL_duty(motor_Loutput);
			
			
			
//******************************************************************************************************			
			
		
		error_R = set_speed_R - motor_Rmeasure;
		integral_R += error_R;

		// 积分限幅
		if (integral_R > 7000.0f) integral_R = 7000.0f;
		if (integral_R < -7000.0f) integral_R = -7000.0f;

		derivative_R = error_R - error_R_last;

		motor_Routput = KP_R * error_R + KI_R * integral_R + KD_R * derivative_R;

		error_R_last = error_R;

		// 输出限幅
		if (motor_Routput > 10000.0f) motor_Routput = 10000.0f;
		if (motor_Routput < -10000.0f) motor_Routput = -10000.0f;

		SetR_duty(motor_Routput);
		
		encoder_r_count = 0;
		encoder_l_count = 0;
		

	
		DL_TimerG_clearInterruptStatus(TIMG0, DL_TIMER_IIDX_ZERO);

	}
}
//获取yaw轴数据中断
void UART1_IRQHandler(void)
{
switch(DL_UART_getPendingInterrupt(UART1))
   {
       case DL_UART_IIDX_RX:
       {
           uint8_t RxData = DL_UART_Main_receiveData(UART1);
           
           if (RxData == '\r' || RxData == '\n')
           {
               if (uart0_rx_index > 0)
               {
                   uart0_rx_buf[uart0_rx_index] = '\0';
                   sscanf(uart0_rx_buf, "%f", &Yaw_received);
                   Yaw_RxFlag = 1;  // 主循环中处理
                   uart0_rx_index = 0;
                   memset(uart0_rx_buf, 0, sizeof(uart0_rx_buf));
               }
           }
           else
           {
               if (uart0_rx_index < sizeof(uart0_rx_buf) - 1)
               {
                   uart0_rx_buf[uart0_rx_index++] = (char)RxData;
               }
               else
               {
                   // 缓冲区溢出，丢弃
                   uart0_rx_index = 0;
                   memset(uart0_rx_buf, 0, sizeof(uart0_rx_buf));
               }
           }
           break;
       }
       default:
           break;
   }
}

//UART0的中断，防止程序意外触发接收中断的时候导致程序卡死的现象
void UART0_IRQHandler(void)
{
	if(DL_UART_getPendingInterrupt(UART0))
	{
		NVIC_ClearPendingIRQ(UART0_INT_IRQn);//清除UART0中断 --> 中断用于调试
	}
}


