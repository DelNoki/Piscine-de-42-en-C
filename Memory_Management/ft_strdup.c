#include <stdlib.h>

char *ft_strdup(char *src)
{
    char *dest;
    int i = 0;
    if (!src)
        return (NULL);
    while (src[i])
        i++;
    dest = (char *)malloc(sizeof(char) * (i + 1));
    i = 0;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return (dest);
}

#include <stdio.h>

int main(int n, char **c)
{
    char *dest;
    if (n == 2)
    {
        dest = ft_strdup(c[1]);
        printf(dest);
        free(dest);
    }
    else
        printf("Error");
    return (0);
}