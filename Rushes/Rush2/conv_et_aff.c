#include "rush2.h"

char *remove_spaces(char *s)
{
    int i = 0;
    int j = 0;
    char *new = (char *)malloc(sizeof(char) * (N * N + 1));
    if (!new)
        return (NULL);
    while (s[i])
    {
        if (s[i] == ' ')
            i++;
        else
        {
            new[j] = s[i];
            j++;
            i++;
        }
    }
    new[j] = 0;
    return (new);
}

void conv(int tab[N][N], char *s)
{
    int i = 0;
    while (s[i])
    {
        tab[i / 4][i % 4] = s[i] - '0';
        i++;
    }
}

void initialisation(int tab[N][N])
{
    int i = 0;
    int j;
    while (i < N)
    {
        j = 0;
        while (j < N)
        {
            tab[i][j] = 0;
            j++;
        }
        i++;
    }
}

void afficher(int tab[N][N])
{
    int i = 0;
    int j;
    char c;
    while (i < N)
    {
        j = 0;
        while (j < N)
        {
            c = tab[i][j] + '0';
            write(1, &c, 1);
            j++;
            if (j < N)
                write(1, " ", 1);
        }
        write(1, "\n", 1);
        i++;
    }
}