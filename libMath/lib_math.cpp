#include "lib_math.h"

namespace bm
{
namespace math
{
    // 求定积分
    double definit_integral(MATH_FUNC fun, double a, double b, int n)
    {
        double s = 0.0;
        double d = (b - a) / n;
        for (int i = 0; i < n; i++)
            s += fun(a + i * d);

        return s;
    }
    // 求微分
    double devide(MATH_DERIV dfun, double x, double a, double b, int n)
    {
        double dx = (b - a) / n;
        return dfun(x) * dx;
    }
}
}