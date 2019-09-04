#include <stdio.h>

int const FIRST_COEFFICIENT = 3;
int const SECOND_COEFFICIENT = 4;
double const DIVISOR = 3;
double const EPSILON = 0.01;

const char INVALID_INPUT[] = "NOT A DOUBLE";

// calculate sin
double calculateSin(double x);


/**
 * main
 * calculate sin(x)
 * @return sin(x)
 */
int main()
{
    double x;
    if (scanf("%lf ", &x) == 1)
    {
        printf("%lf", calculateSin(x));
    }
    else
    {
        fprintf(stderr, INVALID_INPUT);
    }
    return 0;
}

/**
 * sin(x)
 * @param x a number
 * @return sin(X)
 */
double calculateSin(const double x)
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
    double arg1 = calculateSin(x / DIVISOR);
    return FIRST_COEFFICIENT * arg1 - SECOND_COEFFICIENT * arg1 * arg1 * arg1;
}