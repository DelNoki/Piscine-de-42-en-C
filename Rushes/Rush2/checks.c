#include "rush2.h"

int maj_square(int n, int tab[N][N], int x, int y)
{
    int k = 0;
    while (k < N)
    {
        if (k != y && tab[x][k] == n)
            return (0);
        if (k != x && tab[k][y] == n)
            return (0);
        k++;
    }
    return (1);
}

int count_towers(int view[N])
{
    int i = 0;
    int max = 0;
    int count = 0;

    while (i < N)
    {
        if (view[i] > max)
        {
            max = view[i];
            count++;
        }
        i++;
    }
    return (count);
}

int check_line(int tab[N][N], int entree_util[N][N], int i, int j)
{
    int k = 0;
    int r_view[N];
    int l_view[N];
    if (j == N - 1)
    {
        while (k < N)
        {
            l_view[k] = tab[i][k];
            k++;
        }
        if (count_towers(l_view) != entree_util[2][i])
            return (0);
        k = 0;
        while (k < N)
        {
            r_view[k] = tab[i][N - 1 - k];
            k++;
        }
        if (count_towers(r_view) != entree_util[3][i])
            return (0);
    }
    return (1);
}

int check_column(int tab[N][N], int entree_util[N][N], int i, int j)
{
    int k = 0;
    int t_view[N];
    int b_view[N];
    if (i == N - 1)
    {
        while (k < N)
        {
            t_view[k] = tab[k][j];
            k++;
        }
        if (count_towers(t_view) != entree_util[0][j])
            return (0);
        k = 0;
        while (k < N)
        {
            b_view[k] = tab[N - 1 - k][j];
            k++;
        }
        if (count_towers(b_view) != entree_util[1][j])
            return (0);
    }
    return (1);
}