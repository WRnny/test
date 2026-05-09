#ifndef PID_H
#define PID_H

// PID系数结构体 - 用于存储PID的三个增益参数
typedef struct {
    float Kp;    // 比例系数 (Proportional gain)
    float Ki;    // 积分系数 (Integral gain)
    float Kd;    // 微分系数 (Derivative gain)
	float DeadZone; //死去控制的范围 --> 0.02f * (output_max - output_min)	
	float alpha; //一阶滤波系数
} PID_Coefficients;

// PID状态的结构体 - 储存PID计算中的积分和误差
typedef struct{
    float integral;    // 积分项的当前值
    float last_error;  // 上一次的误差值
	float last_measure;//上一次的测量值
	float last_output; //上一次的输出值
	float last_derivative; //上一次微分先行的结果
} PID_interror;


float PID_Calculate(PID_Coefficients* pid_parameters,
                    float measure, float target, 
                    float integral_max, float integral_min, 
                    float output_max, float output_min,
                    PID_interror* pid_interror,
                    float dt);



/************************************************************
 * PID算法实现 - 计算PID控制器的输出值
 * 
 * 参数说明:
 * - pid_parameters: 指向PID_Coefficients结构体的指针，包含Kp、Ki、Kd三个增益参数。
 * - measure: 当前测量值 ，即系统当前状态。
 * - target: 目标值 ，即系统期望达到的状态。
 * - integral_max: 积分项的最大值，用于防止积分过大导致系统不稳定。
 * - integral_min: 积分项的最小值，用于防止积分过小导致系统不稳定。
 * - output_max: 输出值的最大限制，防止控制信号过大。
 * - output_min: 输出值的最小限制，防止控制信号过小。
 * - pid_interror: 指向PID_interror结构体的指针，用于存储积分和上一次误差值。
 * - dt: 时间间隔，用于计算积分和微分项。
 * 
 * 返回值:
 * - 返回计算得到的PID输出值，经过限幅处理后确保在output_min和output_max之间。
 * 
 * 使用方式：在需要PID计算的时候可以直接使用，不过需要搞俩个结构体去放PID参数，积分和上一次误差
 *          
 *             用俩个结构体的原因是：
 *                                  1.不同的PID算法可能需要不同的Kp、Ki、Kd参数
 *                                  2.如果需要多次调用这个PID算法，在函数中使用static去定义积分和上一次误差可能出现问题
 * 
 * 使用示例：
 * 
 *      PID_Coefficients pid_parameters = {
 *          .Kp = 1.0f,
 *          .Ki = 0.1f,
 *          .Kd = 0.01f
 *       };// 初始化PID参数
 *       PID_interror pid_interror = {0.0f, 0.0f}; // 初始化积分和上一次误差为0
 * 
 *      float measure = 测量函数返回的值; // 当前测量值
 * 
 *      out = float PID_Calculate(&pid_parameters,
 *                   float measure, float target, 
 *                    float integral_max, float integral_min, 
 *                    float output_max, float output_min,
 *                    &pid_interror,
 *                    float dt);//获取输出值
 *
 *      ***(out)//设置输出值
 * 
************************************************************/
#endif