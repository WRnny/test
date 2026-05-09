#include "Track.h"
#include "motor.h"
#include "OLED.h"
#include "Alert.h"
//不亮为1亮为0



//int   PIN[7];
//void Read_Pin()
//{
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_0_PIN)==0)
//	{
//	PIN[0]=DL_GPIO_readPins(Track_PORT,Track_PIN_0_PIN);
//	}else PIN[0]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_1_PIN)==0)
//	{
//	PIN[1]=DL_GPIO_readPins(Track_PORT,Track_PIN_1_PIN);
//	}else PIN[1]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_2_PIN)==0)
//	{
//	PIN[2]=DL_GPIO_readPins(Track_PORT,Track_PIN_2_PIN);
//	}else PIN[2]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_3_PIN)==0)
//	{
//	PIN[3]=DL_GPIO_readPins(Track_PORT,Track_PIN_3_PIN);
//	}else PIN[3]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_4_PIN)==0)
//	{
//	PIN[4]=DL_GPIO_readPins(Track_PORT,Track_PIN_4_PIN);
//	}else PIN[4]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_5_PIN)==0)
//	{
//	PIN[5]=DL_GPIO_readPins(Track_PORT,Track_PIN_5_PIN);
//	}else PIN[5]=1;
//	if(DL_GPIO_readPins(Track_PORT,Track_PIN_6_PIN)==0)
//	{
//	PIN[6]=DL_GPIO_readPins(Track_PORT,Track_PIN_6_PIN);
//	}else PIN[6]=1;

//}


////------------------- 常量区 -------------------
//#define INTEGRAL_LIMIT  0
//#define BASE_SPEED      3000
//#define MAX_SPEED       8000      // 先别拉太高
//#define MIN_SPEED      -8000
//#define KP  30.0f
//#define KI   0.1f
//#define KD  30.0f

//// 左负右正（与物理排列一致）
//int WEIGHT[7] = {-100,-50,-20,0,20,50,100};

////------------------- 变量区 -------------------
//static float last_error = 0;
//static float integral   = 0;


////------------------- 主循环函数 -------------------
//void Track_PID(void)
//{

//    Read_Pin();

//    int weighted_sum = 0, sensor_sum = 0;
//    for (int i = 0; i < 7; ++i) {
//        weighted_sum += PIN[i] * WEIGHT[i];
//        sensor_sum   += PIN[i];
//    }
//	OLED_ShowSignedNum(1,1,weighted_sum*40,4);
//	OLED_ShowSignedNum(2,1,sensor_sum,4);

////    /* === 边界 1：全白，掉线 === */
////    if (sensor_sum == 0) {
////        SetL_duty(-BASE_SPEED);
////        SetR_duty(BASE_SPEED);   // 原地左转
////        integral = last_error = 0;
////        return;
////    }

////    /* === 边界 2：全黑 / 几乎全黑 === */
////    if (sensor_sum >= 7) {        // ★ 关键修正
////        SetL_duty(BASE_SPEED);
////        SetR_duty(BASE_SPEED);
////        integral = last_error = 0;
////        return;
////    }


//    /* 调速：error>0 → 右轮减速，左轮加速 */
//    int left  = BASE_SPEED + weighted_sum*KP;
//    int right = BASE_SPEED - weighted_sum*KP;

//    if (left  > MAX_SPEED) left  = MAX_SPEED;
//    if (left  < MIN_SPEED) left  = MIN_SPEED;
//    if (right > MAX_SPEED) right = MAX_SPEED;
//    if (right < MIN_SPEED) right = MIN_SPEED;

//	SetL_duty(left);
//    SetR_duty(right);
//}
//// 在文件顶部添加这些宏定义
//#define SPD_2500 2300
//#define SPD_3000 2800
//#define SPD_4000 3800
//#define SPD_N3000 -3000

//void track()
//{
//    Read_Pin();    
//    // 处理全黑情况（所有传感器都检测到黑线）
//    if(PIN[0]==1 && PIN[1]==1 && PIN[2]==1 && PIN[3]==1 && PIN[4]==1 && PIN[5]==1 && PIN[6]==1)
//    {
//         // 轻微左转
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_3000);
//    }
//    // 处理全白情况（所有传感器都未检测到黑线）
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0)
//    {
//        // 中度左转
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_4000);
//    }
//    // 处理各种组合情况（按优先级从高到低）
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0&&PIN[3] == 1)  // 中间传感器检测到黑线
//    {
//        // 直行
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_2500);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[3]==0 && PIN[5]==0 && PIN[6]==0&&PIN[2] == 1 && PIN[4] == 1)  // 中间两侧同时检测到
//    {
//        // 直行（轻微修正）
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_2500);
//    }else if(PIN[0]==0 && PIN[1]==0 && PIN[3]==1 && PIN[5]==0 && PIN[6]==0&&PIN[2] == 1 && PIN[4] == 1)  // 中间两侧同时检测到
//    {
//        // 直行（轻微修正）
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_2500);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[2] == 1)  // 左中传感器检测到
//    {
//        // 轻微左转
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_3000);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[5]==0 && PIN[6]==0&& PIN[4] == 1)  // 右中传感器检测到
//    {
//        // 轻微右转
//        SetL_duty(SPD_3000);
//        SetR_duty(SPD_2500);
//    }
//    else if(PIN[0]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[1] == 1)  // 左侧传感器检测到
//    {
//        // 中度左转
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_4000);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[6]==0 && PIN[5] == 1)  // 右侧传感器检测到
//    {
//        // 中度右转
//        SetL_duty(SPD_4000);
//        SetR_duty(SPD_2500);
//    }
//    else if( PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[0] == 1)  // 最左侧传感器检测到
//    {
//        // 大角度左转
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000);
//		Buzzer_turn();
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6] == 1)  // 最右侧传感器检测到
//    {
//        // 大角度右转
//        SetL_duty(SPD_4000);
//        SetR_duty(SPD_3000);
//    }
//    // 处理特殊组合情况
//    else if(PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[0] == 1 && PIN[1] == 1)  // 左侧两个传感器检测到
//    {
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000); // 大角度左转
//		Buzzer_turn();
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[4]==0 && PIN[5] == 1 && PIN[6] == 1)  // 右侧两个传感器检测到
//    {
//        // 大角度右转
//        SetL_duty(SPD_4000);
//        SetR_duty(SPD_N3000);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 &&  PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[2] == 1 && PIN[3] == 1)  // 左中和中间检测到
//    {
//        // 轻微左转
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_3000);
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[5]==0 && PIN[6]==0 && PIN[3] == 1 && PIN[4] == 1)  // 中间和右中检测到
//    {
//        // 轻微右转
//        SetL_duty(SPD_3000);
//        SetR_duty(SPD_2500);
//    }
//    else if(PIN[3]==0 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[0] == 1 && PIN[1] == 1 && PIN[2]==1)  // 左侧3个传感器检测到
//    {
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000); // 大角度左转
//		Buzzer_turn();
//    }
//    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==0 && PIN[5] == 1 && PIN[6] == 1 && PIN[4]==1)  // 右侧3个传感器检测到
//    {
//        // 大角度右转
//        SetL_duty(SPD_4000);
//        SetR_duty(SPD_N3000);
//    }
////    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==0 && PIN[3]==1 && PIN[5] == 1 && PIN[6] == 1 && PIN[4]==1)  // 右侧4个传感器检测到
////    {
////        // 大角度右转
////        SetL_duty(SPD_4000);
////        SetR_duty(SPD_N3000);
////    }
//    else if(PIN[3]==1 && PIN[4]==0 && PIN[5]==0 && PIN[6]==0 && PIN[0] == 1 && PIN[1] == 1 && PIN[2]==1)  // 左侧4个传感器检测到
//    {
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000); // 大角度左转
//		Buzzer_turn();
//    }
//    else if(PIN[3]==1 && PIN[4]==1 && PIN[5]==0 && PIN[6]==0 && PIN[0] == 1 && PIN[1] == 1 && PIN[2]==1)  // 左侧5个传感器检测到
//    {
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000); // 大角度左转
//		Buzzer_turn();
//    }
////    else if(PIN[0]==0 && PIN[1]==0 && PIN[2]==1 && PIN[3]==1 && PIN[5] == 1 && PIN[6] == 1 && PIN[4]==1)  // 右侧5个传感器检测到
////    {
////        // 大角度右转
////        SetL_duty(SPD_4000);
////        SetR_duty(SPD_N3000);
////    }
//    else if(PIN[3]==1 && PIN[4]==1 && PIN[5]==1 && PIN[6]==0 && PIN[0] == 1 && PIN[1] == 1 && PIN[2]==1)  // 左侧6个传感器检测到
//    {
//        SetL_duty(SPD_N3000);
//        SetR_duty(SPD_4000); // 大角度左转
//		Buzzer_turn();
//    }
//    else if(PIN[0]==0 && PIN[1]==1 && PIN[2]==1 && PIN[3]==1 && PIN[5] == 1 && PIN[6] == 1 && PIN[4]==1)  // 右侧6个传感器检测到
//    {
//        // 大角度右转
//        SetL_duty(SPD_4000);
//        SetR_duty(SPD_N3000);
//    }
//    // 其他未处理的情况（安全处理）
//    else
//    {
//        // 保持直行
//        SetL_duty(SPD_2500);
//        SetR_duty(SPD_2500);
//    }
//}


//	
