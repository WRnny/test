#include "Track.h"
#include "motor.h"
#include "OLED.h"
#include "Alert.h"
//黑 --> 1	白 --> 0

int pin_number[7];
volatile float coord;
volatile int Flag_coord = 0;
volatile int Flag_Black = 0;
volatile int Flag_RigjtAngle = 0;
volatile int time = 0;
volatile float last_coord = 4.0f;  // 初始化为中心位置

volatile float qty;
uint32_t test;
void track()
{
	//记录传感器检测到黑色的个数
	qty = 0.0f;
	//pin_number[0] = GPIOB->DIN31_0 & DL_GPIO_PIN_4 ? 1 : 0;
	//pin_number[1] = GPIOB->DIN31_0 & Track_PIN_0_PIN ? 1 : 0;
	//获得传感器坐标数据
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_4))	{pin_number[0] = 1; qty++;}	else	pin_number[0] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_12))	{pin_number[1] = 1; qty++;}	else	pin_number[1] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_17))	{pin_number[2] = 1; qty++;}	else	pin_number[2] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_5))	{pin_number[3] = 1; qty++;}	else	pin_number[3] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_13))	{pin_number[4] = 1; qty++;}	else	pin_number[4] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_19))	{pin_number[5] = 1; qty++;}	else	pin_number[5] = 0;
	if(0 < DL_GPIO_readPins(GPIOB,DL_GPIO_PIN_21))	{pin_number[6] = 1; qty++;}	else	pin_number[6] = 0;
	
	//上面调用寄存器是之前的配置文件多了，然后有冲突所以直接调用的寄存器
	
	
	//test = GPIOB->DIN31_0;
	//test = DL_GPIO_readPins(Track_PORT,Track_PIN_5_PIN);
	// 0011 1010 1010 0011 0000 0111 0000
	//         20   16   12    8    4     
	
	//计算传感器坐标
	float sum_pin = 1 * pin_number[0] + 2 * pin_number[1] + 3 * pin_number[2] + 4 * pin_number[3]
					+ 5 * pin_number[4] + 6 * pin_number[5] + 7 * pin_number[6];

	//--------处理特殊情况：全白情况（脱轨状态）--------
	if(0 == qty)
	{
		coord = last_coord;  // 使用上一时刻的位置
		Flag_coord++;
	}
	else
	{
		// 检测全黑标志位
		if(qty == 7)
		{
			Flag_Black = 1;
		}else{
			Flag_Black = 0;
		}
		// 检测直角
		if(pin_number[0] == 1 && pin_number[1] == 1 || pin_number[5] == 1 && pin_number[6] == 1)
		{
			Flag_RigjtAngle = 1;
		}else{
			Flag_RigjtAngle = 0;
		}
		Flag_coord = 0;
		coord = sum_pin / (qty * 1.0);  // 正常计算位置
		last_coord = coord;  // 记录当前时刻位置，防止脱轨找不到方向
		qty = 0;
	}
}



//// 传感器状态数组，不亮为1，亮为0
//int PIN[7];

//// 读取传感器状态
//void Read_Pin()
//{
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_0_PIN)==0)
//    {
//        PIN[0]=DL_GPIO_readPins(Track_PORT,Track_PIN_0_PIN);
//    }else PIN[0]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_1_PIN)==0)
//    {
//        PIN[1]=DL_GPIO_readPins(Track_PORT,Track_PIN_1_PIN);
//    }else PIN[1]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_2_PIN)==0)
//    {
//        PIN[2]=DL_GPIO_readPins(Track_PORT,Track_PIN_2_PIN);
//    }else PIN[2]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_3_PIN)==0)
//    {
//        PIN[3]=DL_GPIO_readPins(Track_PORT,Track_PIN_3_PIN);
//    }else PIN[3]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_4_PIN)==0)
//    {
//        PIN[4]=DL_GPIO_readPins(Track_PORT,Track_PIN_4_PIN);
//    }else PIN[4]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_5_PIN)==0)
//    {
//        PIN[5]=DL_GPIO_readPins(Track_PORT,Track_PIN_5_PIN);
//    }else PIN[5]=1;
//    if(DL_GPIO_readPins(Track_PORT,Track_PIN_6_PIN)==0)
//    {
//        PIN[6]=DL_GPIO_readPins(Track_PORT,Track_PIN_6_PIN);
//    }else PIN[6]=1;
//}

////------------------- 常量区 -------------------
//#define BASE_SPEED      3000
//#define MAX_SPEED       8000
//#define MIN_SPEED      -8000
//#define KP  30.0f

//// 左负右正（与物理排列一致），以传感器3为中心(索引3对应位置0)
//int WEIGHT[7] = {-300,-200,-100,0,100,200,300};

////------------------- 变量区 -------------------
//static float last_error = 0;
//static float integral   = 0;

////------------------- PID循迹函数 -------------------
//void Track_PID(void)
//{
//    Read_Pin();

//    int weighted_sum = 0, sensor_sum = 0;
//    for (int i = 0; i < 7; ++i) {
//        weighted_sum += PIN[i] * WEIGHT[i];
//        sensor_sum   += PIN[i];
//    }

//    // 显示传感器状态用于调试
//    OLED_ShowSignedNum(1,1,weighted_sum/100,4);
//    OLED_ShowSignedNum(2,1,sensor_sum,4);

//    // 边界1：全白，掉线
//    if (sensor_sum == 0) {
//        SetL_duty(-BASE_SPEED);
//        SetR_duty(BASE_SPEED);   // 原地左转寻找黑线
//        integral = last_error = 0;
//        return;
//    }

//    // 边界2：全黑 / 几乎全黑
//    if (sensor_sum >= 6) {
//        SetL_duty(BASE_SPEED);
//        SetR_duty(BASE_SPEED);
//        integral = last_error = 0;
//        return;
//    }

//    // PID计算：error>0 → 右轮减速，左轮加速
//    float error = (float)weighted_sum / sensor_sum; // 归一化误差
//    integral += error;
//    float derivative = error - last_error;
//    last_error = error;

//    int correction = (int)(error * KP + integral * 0.1f + derivative * 10.0f);

//    int left  = BASE_SPEED + correction;
//    int right = BASE_SPEED - correction;

//    // 限幅
//    if (left  > MAX_SPEED) left  = MAX_SPEED;
//    if (left  < MIN_SPEED) left  = MIN_SPEED;
//    if (right > MAX_SPEED) right = MAX_SPEED;
//    if (right < MIN_SPEED) right = MIN_SPEED;

//    SetL_duty(left);
//    SetR_duty(right);
//}

//// 简化的位置计算算法（改进版）
//float CalculatePosition(int* pin_number)
//{
//    int active_sensors = 0;
//    float weighted_sum = 0.0f;

//    // 使用对称权重：-3, -2, -1, 0, 1, 2, 3
//    int symmetric_weights[7] = {-3, -2, -1, 0, 1, 2, 3};

//    for (int i = 0; i < 7; i++) {
//        if (pin_number[i] == 0) { // 检测到黑线
//            active_sensors++;
//            weighted_sum += symmetric_weights[i];
//        }
//    }

//    // 避免除以零
//    if (active_sensors == 0) {
//        return 0.0f; // 没有检测到黑线，返回中心位置
//    }

//    // 返回归一化的位置值：-3到+3之间
//    return weighted_sum / active_sensors;
//}

//// 基于位置控制的循迹函数
//void TrackByPosition(void)
//{
//    Read_Pin();

//    float position = CalculatePosition(PIN);

//    // 显示位置信息
//    OLED_ShowSignedNum(3,1,(int)(position * 100),4);

//    // 简单的P控制
//    int correction = (int)(position * KP);

//    int left  = BASE_SPEED + correction;
//    int right = BASE_SPEED - correction;

//    // 限幅
//    if (left  > MAX_SPEED) left  = MAX_SPEED;
//    if (left  < MIN_SPEED) left  = MIN_SPEED;
//    if (right > MAX_SPEED) right = MAX_SPEED;
//    if (right < MIN_SPEED) right = MIN_SPEED;

//    SetL_duty(left);
//    SetR_duty(right);
//}