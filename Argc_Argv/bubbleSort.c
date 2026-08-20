/*
Code qui trie les entrées utilisateur par ordre ascii croissant
*/

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

void ascii_inc_sort(int size, char **tab)
{
    int i = 0;
    char *swap;
    while (i < size - 1)
    {
        if (ft_strcmp(tab[i], tab[i + 1]) > 0)
        {
            swap = tab[i];
            tab[i] = tab[i + 1];
            tab[i + 1] = swap;
            i = 0;
        }
        else
            i++;
    }
}

#include <stdio.h>
int main(int n, char **c)
{
    int i = 0;
    int nb = n;
    char **tab;
    if (nb >= 2)
    {
        tab = c + 1;
        ascii_inc_sort(nb - 1, tab);
        while (i < nb - 1)
        {
            printf("%s\n", tab[i]);
            i++;
        }
    }
    else
        printf("Wrong number of arguments.\n");
    return (0);
}