#include "PerlinGeneration.h"
#include "Common.h"
#include "CustomMath.h"

using PG = PerlinGeneration;

static inline double Fade(double t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

static inline double Lerp(double a, double b, double t) {
    return a + t * (b - a);
}

double PG::DotGridGradient(int ix, int iy, double x, double y) {
    vector2 gradient = RandomGradient(ix, iy);

    vector2 distance;
    distance.x = x - static_cast<double>(ix);
    distance.y = y - static_cast<double>(iy);

    return CustomMath::DotProduct2D(gradient, distance);
}

vector2 PG::RandomGradient(int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = static_cast<unsigned>(ix);
    unsigned b = static_cast<unsigned>(iy);

    a *= 3284157443u;
    b ^= (a << s) | (a >> (w - s));
    b *= 1911520717u;
    a ^= (b << s) | (b >> (w - s));
    a *= 2048419325u;

    double random = a * (3.14159265358979323846 / 2147483648.0);

    vector2 v;
    v.x = CustomMath::Cos(random);
    v.y = CustomMath::Sin(random);

    return v;
}

double PG::PerlinNoise(double x, double y) {
    int x0 = static_cast<int>(CustomMath::Floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(CustomMath::Floor(y));
    int y1 = y0 + 1;

    double sx = x - static_cast<double>(x0);
    double sy = y - static_cast<double>(y0);

    double n00 = PG::DotGridGradient(x0, y0, x, y);
    double n10 = PG::DotGridGradient(x1, y0, x, y);
    double n01 = PG::DotGridGradient(x0, y1, x, y);
    double n11 = PG::DotGridGradient(x1, y1, x, y);

    double u = Fade(sx);
    double v = Fade(sy);

    double ix0 = Lerp(n00, n10, u);
    double ix1 = Lerp(n01, n11, u);

    return Lerp(ix0, ix1, v);
}