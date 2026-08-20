#include <stdlib.h>

int is_sep(char *charsep, char c)
{
    int i = 0;
    while (charsep[i])
    {
        if (c == charsep[i])
            return (1);
        i++;
    }
    return (0);
}

int ft_strlen(char *charsep, char *src, int index)
{
    int i = 0;
    if (src[i + index] && !is_sep(charsep, src[i + index]))
    {
        while (src[i + index] && !is_sep(charsep, src[i + index]))
            i++;
    }
    else if (src[i + index] && is_sep(charsep, src[i + index]))
    {
        while (src[i + index] && is_sep(charsep, src[i + index]))
            i++;
    }
    return (i);    
}

int count_words(char *src, char *charsep)
{
    int i = 0;
    int count = 0;
    int word = 0;
    while (src[i])
    {
        if (word == 0 && !is_sep(charsep, src[i]))
        {
            word = 1;
            count++;
        }
        else if (word == 1 && is_sep(charsep, src[i]))
            word = 0;
        i++;
    }
    return (count);
}

char *ft_strdup(char *src, int index, int size)
{
    char *dest;
    int i = 0;
    dest = (char *)malloc(sizeof(char) * (size + 1));
    while (i < size)
    {
        dest[i] = src[index + i];
        i++;
    }
    dest[i] = 0;
    return (dest);
}

char **ft_split(char *src, char *charsep)
{
    int i = 0;
    int j = 0;
    int l;
    char **tab = (char **)malloc(sizeof(char *) * (count_words(src, charsep) + 1));
    if (!tab)
        return (NULL);
    while (src[i])
    {
        l = ft_strlen(charsep, src, i);
        if (!is_sep(charsep, src[i]))
        {
            tab[j] = ft_strdup(src, i, l);
            if (!(tab[j]))
            {
                while (j > 0)
                {
                    free(tab[j]);
                    j--;
                }
                return (NULL);
            }
            j++;
        }
        i += l;
    }
    tab[j] = NULL;
    return (tab);
}


#include <stdio.h>

int main(int n, char **c)
{
    char *charsep;
    char *src;
    char **tab;
    int i;
    if (n >= 1 && n <= 3)
    {
        if (n == 2)
        {
            src = c[1];
            printf(src);
        }
        else
        {
            charsep = c[1];
            src = c[2];
            tab = ft_split(src, charsep);
            i = 0;
            while (tab[i] != NULL)
            {
                printf(tab[i]);
                printf("\n");
                free(tab[i]);
                i++;
            }
            free(tab);
        } 
    }
    else
        printf("Error\n");
    return (0);
}