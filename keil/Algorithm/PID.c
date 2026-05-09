#include "PID.h"
#include <math.h>

//计算PID输出值
float PID_Calculate(PID_Coefficients* pid_parameters,
                    float measure, float target, 
                    float integral_max, float integral_min, 
                    float output_max, float output_min,
                    PID_interror* pid_interror,
                    float dt)
{
	//输出值
	float output;
	
    //计算当前误差值
    float now_error = target - measure;
	
	//增加死区控制，在现在的循迹小车的方案可能用不到
	if(fabs(now_error) < pid_parameters->DeadZone)	now_error = 0;
	
	//变速积分（积分分离+梯形积分）
	float integral_Low = 0.2 * fabs(integral_max);
	float integral_Up = 0.85 * fabs(integral_max);
	
	//小误差 -> 全积分
	if(fabs(now_error) < integral_Low)
	{
		pid_interror->integral += (now_error + pid_interror->last_error) / 2.0;
	}
	//中误差 -> 部分积分
	else if(fabs(now_error) < integral_Up)
	{
		pid_interror->integral += ( (now_error + pid_interror->last_error) / 2.0 ) * ( (now_error - integral_Low) / (integral_Up - integral_Low) );
	}
	//大误差 -> 关闭积分项
	else
	{
		pid_interror->integral = 0;
	}
	
    //计算积分项 --> 积分分离
//    pid_interror->integral += now_error * dt;
	
    //积分限幅
//    if (pid_interror->integral > integral_max)
//    {
//        pid_interror->integral = integral_max;
//    }
//    else if (pid_interror->integral < integral_min)
//    {
//        pid_interror->integral = integral_min;
//    }
	
	pid_interror->integral = fmaxf(integral_min, fminf(pid_interror->integral, integral_max));
	
	//进行微分先行(0.7测量值 + 0.3输出值) --> 微分先行是为了防止目标值突变的时候误差过大，导致超调的现象
	float now_derivative = -0.7f * (measure - pid_interror->last_measure) / dt + 0.3f * (pid_interror->last_output - output);
	pid_interror->last_measure = measure;
	//效果：消除目标值突变引起的微分冲击（如循迹小车急转弯时更平稳）

//  //计算微分项  --> 改用微分先行，虽然循迹小车中不涉及目标值突变的问题
//  float derivative = (now_error - pid_interror->last_error) / dt;
//  pid_interror->last_error = now_error;
	
	//不完全微分
	float derivative = pid_parameters->alpha * pid_interror->last_derivative + (1 - pid_parameters->alpha) * now_derivative;
	pid_interror->last_derivative = now_derivative;

    output = pid_parameters->Kp * now_error + pid_parameters->Ki * pid_interror->integral 
                    + pid_parameters->Kd * derivative;
	
	pid_interror->last_output = output;
	
//    //输出限幅
//    if(output > output_max)
//    {
//        output = output_max;
//    }
//    else if(output < output_min)
//    {
//        output = output_min;
//    }

	//直接用math中的函数进行限幅减小代码在阅读上的难度
    return fmaxf(output_min, fminf(output, output_max));
}
