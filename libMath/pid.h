#ifndef __PID_H__
#define __PID_H__

typedef struct _pid
{
    double setValue;            // 设置值
    double atualValue;          // 实际值
    double deviation;           // 偏差值
    double deviationLast;       // 上一个偏差值
    double deviationNext;       // 下一个偏差值
    double kp, ki, kd;          // 比例、积分、微分系数
    double integral;            // 积分值
    double value;               // 值
    void positonInit();
    void incrementInit();
} pidValue;

// 位置型pid调节
double positonPID(double value, pidValue& v);
// 增量型pid调节
double incrementPID(double value, pidValue& v);
#endif