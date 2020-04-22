#include "pid.h"

void _pid::positonInit()
{
    setValue = 0.0;
    atualValue = 0.0;
    deviation = 0.0;
    deviationLast = 0.0;
    kp = 0.2;
    ki = 0.015;
    kd = 0.2;
    integral = 0.0;
    value = 0.0;
}
void _pid::incrementInit()
{
    setValue = 0.0;
    atualValue = 0.0;
    deviation = 0.0;
    deviationLast = 0.0;
    deviationNext = 0.0;
    kp = 0.2;
    ki = 0.015;
    kd = 0.2;
}
// 位置型pid调节
double positonPID(double value, pidValue& v)
{
    v.setValue = value;
    v.deviation = v.setValue - v.atualValue;
    v.integral += v.deviation;
    v.value = v.kp * v.deviation + v.ki * v.integral + v.kd * (v.deviation - v.deviationLast);
    v.deviationLast = v.deviation;
    v.atualValue = v.value * 1.0;

    return v.atualValue;
}
// 增量型pid调节
double incrementPID(double value, pidValue& v)
{
    v.setValue = value;
    v.deviation = v.setValue - v.atualValue;
    double incretment = v.kp * (v.deviation - v.deviationNext) + v.ki * v.deviation + v.kd * (v.deviation - 2 * v.deviationNext + v.deviationLast);
    v.atualValue += incretment;
    v.deviationLast = v.deviationNext;
    v.deviationNext = v.deviation;
    return v.atualValue;
}