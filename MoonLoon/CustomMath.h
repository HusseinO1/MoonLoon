#pragma once
#include "Common.h"
class CustomMath
{
public:
	static double DotProduct2D(vector2 a, vector2 b);
	static double Abs(double v);
	static double Round(double v);
	static double Min(double a, double b);
	static double Max(double a, double b);
	static int	  Min(int a, int b);
	static int	  Max(int a, int b);
	static double Mod(double num, double denom);
	static double Sin(double x);
	static double Cos(double x);
	static double Sqrt(double x);
	static double Pow(double base, int exponent);
	static double Exp(double x);
	static double CubicInterpolation(double a, double b, double t);
	static double Lerp(double a, double b, double t);
	static double Ldexp(double x, int exp);
	static double Ceil(double x);
	static double Floor(double x);
	static double Clamp(double x, double a, double b);
};

