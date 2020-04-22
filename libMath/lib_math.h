#ifndef __LIB_MATH_H__
#define __LIB_MATH_H__
namespace bm
{
namespace math
{
    // 原函数
    typedef double (*MATH_FUNC)(double x);
    // 导函数
    typedef double (*MATH_DERIV)(double x);
    // 求定积分
    double definit_integral(MATH_FUNC fun, double a, double b, int n);
    // 求x的微分
    double devide(MATH_DERIV dfun, double x, double a, double b, int n);
}
}
#endif