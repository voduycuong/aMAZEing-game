// https://www.geeksforgeeks.org/square-root-of-a-number-without-using-sqrt-function/
#include <math.h>

double square(double n, double i, double j)
{
    double mid = (i + j) / 2;
    double mul = mid * mid;

    if ((mul == n) || (fabs(mul - n) < 0.00001))
        return mid;

    else if (mul < n)
        return square(n, mid, j);

    else
        return square(n, i, mid);
}

double sqrt(double n)
{
    double i = 1;

    double result;

    int found = 0;
    while (!found)
    {
        if (i * i == n)
        {
            result = i;
            found = 1;
        }
        else if (i * i > n)
        {
            double res = square(n, i - 1, i);
            result = res;
            found = 1;
        }
        i++;
    }
    return result;
}

double fabs(double x)
{
    if (x >= 0)
        return x;
    else
    {
        double temp = x;
        if (x < 0)
            temp *= -1;
        return temp;
    }
}

double distance(int x1, int x2, int y1, int y2)
{
    double temp;
    temp = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    temp = sqrt(temp);
    return temp;
}