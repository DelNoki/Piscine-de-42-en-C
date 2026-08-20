#ifndef RUSH2_H
#define RUSH2_H

#define N 4
#include <unistd.h>
#include <stdlib.h>

char *remove_spaces(char *s);
void conv(int tab[N][N], char *s);
void initialisation(int tab[N][N]);
void afficher(int tab[N][N]);
int maj_square(int n, int tab[N][N], int x, int y);
int count_towers(int view[N]);
int check_line(int tab[N][N], int entree_util[N][N], int i, int j);
int check_column(int tab[N][N], int entree_util[N][N], int i, int j);
int solution(int i, int j, int tab[N][N], int entree_util[N][N]);

#endif