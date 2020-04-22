#ifndef __LIB_MATH_H__
#define __LIB_MATH_H__
namespace bm
{
namespace math
{
    // ԭ����
    typedef double (*MATH_FUNC)(double x);
    // ������
    typedef double (*MATH_DERIV)(double x);
    // �󶨻���
    double definit_integral(MATH_FUNC fun, double a, double b, int n);
    // ��x��΢��
    double devide(MATH_DERIV dfun, double x, double a, double b, int n);
}
}
#endif