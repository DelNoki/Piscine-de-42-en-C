#include <stdlib.h>

int ft_strcat(char *src, char *dest, int index)
{
    int i = 0;
    while (src[i])
    {
        *(dest + index + i) = src[i];
        i++;
    }
    return (i);
}

int long_tot(char **tab, char *sep)
{
    int i = 0;
    int j;
    int count = 0;
    int long_sep = 0;
    while (sep[i])
        i++;
    long_sep = i;
    i = 0;
    while (tab[i] != NULL)
    {
        j = 0;
        while (tab[i][j])
            j++;
        count += j;
        i++;
        if (tab[i] != NULL)
            count += long_sep;
    }
    return (count);
}

char *ft_strjoin(int size, char **tab, char *sep)
{
    int i = 0;
    int j = 0;
    int l = long_tot(tab, sep);
    char *r = (char *)malloc(sizeof(char) * (l + 1));
    if (!r || size == 0)
        return (NULL);
    while (i < size)
    {
        j += ft_strcat(tab[i], r, j);
        i++;
        if (i < size)
            j += ft_strcat(sep, r, j);
    }
    r[j] = 0;
    return (r);
}


#include <stdio.h>
int main(int n, char **s)
{
    char **tab;
    char *res;
    char *sep;
    if (n <= 2)
        printf("Error\n");
    else
    {
        tab = s + 2;
        sep = s[1];
        res = ft_strjoin(n - 2, tab, sep);
        printf(res);
        free(res);
    }
    return (0);
}