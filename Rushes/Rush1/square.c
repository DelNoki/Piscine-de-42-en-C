#include <unistd.h>

void print_line(char a, char b, char c, int n)
{
    int j = 0;
    while (j < n)
    {
        if (j == 0)
            write(1, &a, 1);
        else if (j == n - 1)
            write(1, &c, 1);
        else
            write(1, &b, 1);
        j++;
    }
}

void print_rect(int x, int y)
{
    int i = 0;
    if (x <= 0 || y <= 0)
    {
        write(1, "Error\n", 6);
        return;
    }
    while (i < x)
    {
        if (i == 0 || i == x - 1)
            print_line('A', 'B', 'C', y);
        else
            print_line('B', ' ', 'B', y);
        write(1, "\n", 1);
        i++;
    }
}


#include <stdlib.h>
int main(int n, char **c)
{
    char *s1;
    char *s2;
    if (n == 3)
    {
        s1 = c[1];
        s2 = c[2];
        print_rect(atoi(s1), atoi(s2));
    }
    else
        write(1, "Wrong number of arguments\n", 26);
    return (0);
}