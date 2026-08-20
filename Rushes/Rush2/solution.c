#include "rush2.h"

int solution(int i, int j, int tab[N][N], int entree_util[N][N])
{
    int n = 1;
    if (i == N)
        return (1);
    if (j == N)
        return (solution(i + 1, 0, tab, entree_util));
    while (n <= N)
    {
        if (maj_square(n, tab, i, j))
        {
            tab[i][j] = n;
            if (check_column(tab, entree_util, i, j) && 
            check_line(tab, entree_util, i, j))
            {
                if (solution(i, j + 1, tab, entree_util))
                    return (1);
            }
        }
        tab[i][j] = 0;
        n++;
    }
    return (0);
}