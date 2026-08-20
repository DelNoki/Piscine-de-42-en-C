#include <stdlib.h>

int is_base_valid(char *base)
{
    int i = 0;
    int j;
    while (base[i])
    {
        if ((base[i] >= 1 && base[i] <= 32) || base[i] == 127 || base[i] == '-' || base[i] == '+')
            return (0);
        j = i + 1;
        while (base[j])
        {
            if (base[j] == base[i])
                return (0);
            j++;
        }
        i++;
    }
    if (i == 0 || i == 1)
        return (0);
    else
        return (1);
}

int is_number_valid(char *nb, char *base)
{
    int i = 0;
    int j;
    int is_found;
    while (nb[i] == ' ')
        i++;
    while (nb[i] == '-' || nb[i] == '+')
        i++;
    while (nb[i])
    {
        is_found = 0;
        j = 0;
        while (base[j])
        {
            if (base[j] == nb[i])
            {
                is_found = 1;
                break;
            }
            j++;
        }
        if (is_found == 0)
            return (0);
        i++;
    }
    return (1);
}

int ft_atoi(char *nb, char *base_from)
{
    int i = 0;
    int j;
    int l;
    int minus = 0;
    int tot = 0;
    if (!is_number_valid(nb, base_from))
        return (0);
    while (base_from[i])
        i++;
    l = i;
    i = 0;
    while (nb[i] == ' ')
        i++;
    while (nb[i] == '-' || nb[i] == '+')
    {
        if (nb[i] == '-')
            minus++;
        i++;
    }
    while (nb[i])
    {
        j = 0;
        while (base_from[j])
        {
            if (base_from[j] == nb[i])
                break;
            j++;
        }
        tot = tot * l + j;
        i++;
    }
    if (minus % 2 == 1)
        tot = -tot;
    return (tot);
}

char *ft_itoa(int nb, char *base_to)
{
    int i = 0;
    int l;
    int neg = 0;
    int n = nb;
    int size = 0;
    char *res;
    while (base_to[i])
        i++;
    l = i;
    while (n != 0)
    {
        n = n / l;
        size++;
    }
    n = nb;
    if (n < 0)
    {
        n = -n;
        neg = 1;
    }
    res = (char *)malloc(sizeof(char) * (size + neg + 1));
    if (!res)
        return (NULL);
    if (neg == 1)
        res[0] = '-';
    i = 0;
    while (i < size)
    {
        res[size + neg - 1 - i] = base_to[n % l];
        n = (n - (n % l)) / l;
        i++;
    }
    res[i + neg] = 0;
    return (res);
}

char *ft_conv_base(char *nb, char *base_from, char *base_to)
{
    if (!is_base_valid(base_from) || !is_base_valid(base_to) || !is_number_valid(nb, base_from))
        return (NULL);
    else
        return(ft_itoa(ft_atoi(nb, base_from), base_to));
}


#include <stdio.h>
int main(void)
{
    char *test = "-7";
    char *base_from = "0123456789";
    char *base_to = "01";
    printf(ft_conv_base(test, base_from, base_to));
    /*
    int n = ft_atoi(test, base_from);
    char *res = ft_itoa(n, base_to);
    printf("%d\n", n);
    printf("%s\n", res);
    */
    return 0;
}