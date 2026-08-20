int it_fact(int n)
{
    int tot = 1;
    int nb = n;
    if (nb < 0)
        return (-1);
    if (nb == 0)
        return (1);
    while (nb >= 1)
    {
        tot = tot * nb;
        nb--;
    }
    return (tot);
}

int rec_fact(int n)
{
    int nb = n;
    if (n < 0)
        return (-1);
    else if (n == 0)
        return (1);
    else
        return (rec_fact(n - 1) * nb);
}


#include <stdio.h>
int main(void)
{
    int n = 7;
    int i = 0;
    while (i < n)
    {
        printf("%d\n", it_fact(i));
        i++;
    }
    i = 0;
    while (i < n)
    {
        printf("%d\n", rec_fact(i));
        i++;
    }
    return (0);
}