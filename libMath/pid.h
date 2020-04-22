#ifndef __PID_H__
#define __PID_H__

typedef struct _pid
{
    double setValue;            // ����ֵ
    double atualValue;          // ʵ��ֵ
    double deviation;           // ƫ��ֵ
    double deviationLast;       // ��һ��ƫ��ֵ
    double deviationNext;       // ��һ��ƫ��ֵ
    double kp, ki, kd;          // ���������֡�΢��ϵ��
    double integral;            // ����ֵ
    double value;               // ֵ
    void positonInit();
    void incrementInit();
} pidValue;

// λ����pid����
double positonPID(double value, pidValue& v);
// ������pid����
double incrementPID(double value, pidValue& v);
#endif