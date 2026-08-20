#include "rush2.h"

int is_entree_ok(int tab[N][N])
{
    int i = 0;
    int j;
    while (i < N)
    {
        j = 0;
        while (j < N)
        {
            if (tab[i][j] < 1 || tab[i][j] > 4)
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

#include <stdio.h>
int main(int n, char **c)
{
    char *s;
    int entree_util[N][N];
    int tab[N][N];
    if (n == 2)
    {
        s = remove_spaces(c[1]);
        //printf("%s\n", s);
        initialisation(tab);
        conv(entree_util, s);
        //printf("%d\n", is_entree_ok(entree_util));
        if (is_entree_ok(entree_util))
        {
            if (solution(0, 0, tab, entree_util))
            {
                write(1, "A solution has been found : \n", 29);
                afficher(tab);
            }
            else
                write(1, "No solution has been found.\n", 29);
        }
        else
            write(1, "Wrong entrees.\n", 15);
    }
    else
        write(1, "Wrong number of arguments.\n", 26);
    return (0);
}