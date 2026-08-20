int it_power(int n, int p)
{
    int tot = 1;
    int d = p;
    if (d < 0)
        return (0);
    if (d == 0)
        return (1);
    while (d >= 1)
    {
        tot = tot * n;
        d--;
    }
    return (tot);
}

int rec_power(int n, int p)
{
    int d = p;
    if (d < 0)
        return (0);
    else if (d == 0)
        return (1);
    else
        return (rec_power(n, d - 1) * n);
}


#include <stdio.h>
int main(void)
{
    int n = 2;
    int max = 10;
    int i = 0;
    while (i < max)
    {
        printf("%d\n", it_power(n, i));
        i++;
    }
    i = 0;
    while (i < max)
    {
        printf("%d\n", rec_power(n, i));
        i++;
    }
    return (0);
}