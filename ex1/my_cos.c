#include <stdio.h>

int const COEFFICIENT_FIRST = 3;
int const COEFFICIENT_SECOND = 4;
double const SIN_DIVISOR = 3;
double const PI = 3.141529;
double const EPSILON = 0.01;

const char NOT_DOUBLE[] = "NOT A DOUBLE";

//calculate sin
double calculateSine(double x);

//calculate cos
double calculateCos(double x);

/**
 * main
 * calculate cos(X)
 * @return cos(x)
 */
int main()
{
    double x;
    if (scanf("%lf ", &x) == 1)
    {
        printf("%lf", calculateCos(x));
    }
    else
    {
        fprintf(stderr, NOT_DOUBLE);
    }
    return 0;
}

/**
 * cos(x)
 * @param x a number
 * @return cos(x)
 */
double calculateCos(const double x)
{
    return calculateSine((PI / 2.0) - x);
}

/**
 * sin(x)
 * @param x a number
 * @return sin(x)
 */
double calculateSine(const double x)
{
    if (x < 0)
    {
        double absX = x * (-1);
        if (absX < EPSILON)
        {
            return x;
        }
    }
    else if (x < EPSILON)
    {
        return x;
    }
    double arg1 = calculateSine(x / SIN_DIVISOR);
    return COEFFICIENT_FIRST * arg1 - COEFFICIENT_SECOND * arg1 * arg1 * arg1;
}