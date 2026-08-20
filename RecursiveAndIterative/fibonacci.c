int ft_fibonacci(int n)
{
    int nb = n;
    if (nb < 0)
        return (-1);
    else if (nb == 0)
        return (0);
    else if (nb == 1)
        return (1);
    else
        return (ft_fibonacci(nb - 1) + ft_fibonacci(nb - 2));
}

#include <stdio.h>
int main(void)
{
    int n = 7;
    int i = 0;
    while (i < n)
    {
        printf("%d\n", ft_fibonacci(i));
        i++;
    }
    return (0);
}