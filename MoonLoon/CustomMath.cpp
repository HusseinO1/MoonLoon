#include "CustomMath.h"
#include "Common.h"

double CustomMath::DotProduct2D(vector2 a, vector2 b)
{
	return a.x * b.x + a.y * b.y;
}

double CustomMath::Abs(double v)
{
    return (v < 0.0) ? -v : v;
}

double CustomMath::Round(double v)
{
	return (v - static_cast<int64>(v) >= 0.5) ? static_cast<int64>(v) + 1 : static_cast<int64>(v);
}

double CustomMath::Min(double a, double b)
{
	return (a < b) ? a : b;
}

double CustomMath::Max(double a, double b)
{
    return (a > b) ? a : b;
}

int CustomMath::Min(int a, int b)
{
    return (a < b) ? a : b;
}

int CustomMath::Max(int a, int b)
{
    return (a > b) ? a : b;
}

double CustomMath::Mod(double num, double denom)
{
    if (denom == 0.0) return 0.0;
    int64 quotient = static_cast<int64>(num / denom);
    return num - (quotient * denom);
}

double CustomMath::Sin(double x)
{
    const double PI = 3.14159265358979323846;
    const double TWO_PI = 2.0 * PI;

    x = CustomMath::Mod(x, TWO_PI);

    if (x > PI)
    {
        x -= TWO_PI;
    }
    else if (x < -PI)
    {
        x += TWO_PI;
    }
    double term = x; 
    double sum = x;
    double xSquared = x * x;

    for (int n = 1; n <= 10; ++n)
    {
        double denominator = (2.0 * n) * (2.0 * n + 1.0);
        term = -term * xSquared / denominator;
        sum += term;

        if (CustomMath::Abs(term) < 1e-15)
        {
            break;
        }
    }

    return sum;
}

double CustomMath::Cos(double x) {
    const double PI = 3.14159265358979323846;
    const double TWO_PI = 2.0 * PI;

    x = CustomMath::Mod(x, TWO_PI);

    if (x > PI)
    {
        x -= TWO_PI;
    }
    else if (x < -PI)
    {
        x += TWO_PI;
    }

    double term = 1.0;
    double sum = 1.0;

    double xSquared = x * x;

    for (int n = 1; n <= 10; ++n)
    {
        double denominator = (2.0 * n - 1.0) * (2.0 * n);

        term = -term * xSquared / denominator;
        sum += term;

        if (CustomMath::Abs(term) < 1e-15) 
        {
            break;
        }
    }

    return sum;
}

double CustomMath::Sqrt(double x)
{
    if (x <= 0.0) return 0.0;

    double guess = x;
    if (guess > 1.0) guess = x * 0.5;

    for (int i = 0; i < 10; ++i)
    {
        guess = 0.5 * (guess + x / guess);
    }

    return guess;
}

double CustomMath::Pow(double base, int exponent)
{
    if (exponent == 0) return 1.0;
    if (exponent < 0)
    {
        base = 1.0 / base;
        exponent = -exponent;
    }

    double result = 1.0;
    while (exponent > 0)
    {
        if (exponent & 1) 
        { 
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return result;
}

double CustomMath::Exp(double x)
{
    const double LN2 = 0.6931471805599453;
    const double INV_LN2 = 1.4426950408889634;

    int k = static_cast<int>(CustomMath::Round(x * INV_LN2));
    double r = x - k * LN2;

    double r2 = r * r;
    double p = 1.0 + r + (r2 * 0.5) * (1.0 + r / 3.0 * (1.0 + r * 0.25 * (1.0 + r * 0.2)));

    return Ldexp(p, k);
}

double CustomMath::CubicInterpolation(double a, double b, double t)
{
	return (b - a) * t * t * (3.0 - 2.0 * t) + a;
}

double CustomMath::Lerp(double a, double b, double t) {
    return a + t * (b - a);
}

double CustomMath::Ldexp(double x, int exp)
{
    if (x == 0.0 || exp == 0) 
    {
        return x;
    }

    while (exp > 1023) 
    {
        x *= 8.9884656743115795e+307;
        exp -= 1023;
    }

    while (exp < -1022)
    {
        x *= 2.2250738585072014e-308;
        exp += 1022;
    }

    union
    {
        uint64 bits;
        double factor;
    } converter;

    converter.bits = static_cast<uint64>(exp + 1023) << 52;

    return x * converter.factor;
}

double CustomMath::Ceil(double x)
{
    return static_cast<double>(static_cast<int64>(x) + 1);
}

double CustomMath::Floor(double x)
{
    return static_cast<double>(static_cast<int64>(x));
}

double CustomMath::Clamp(double x, double a, double b)
{
    return (x < a) ? a : (x > b ? b : x);
}
