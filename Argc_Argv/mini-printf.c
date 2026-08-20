#include <stdarg.h>
#include <unistd.h>

int imprimer_entier(int n) 
{
	char c;
	int count = 0;
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
    if (n < 0) 
	{
		c = '-';
		write(1, &c, 1);
		count++;
        n = -n;
    }
    if (n / 10) 
	{
        count += imprimer_entier(n / 10);
    }
	c = (n % 10) + '0';
	write(1, &c, 1);
	count++;

	return (count);
}

int imprimer_hexadecimal(unsigned int n) 
{
    int count = 0;
    char *base_hex = "0123456789abcdef";
	char c;
    
    if (n / 16) 
	{
        count += imprimer_hexadecimal(n / 16);
    }
	c = base_hex[n % 16];
    write(1, &c, 1);
    count++;
    
    return count;
}

int mini_printf(const char *format, ...)
{
    va_list args;
    int caracteres_ecrits = 0;
	char c;
	char *s;
	int d;
	unsigned int x;

    va_start(args, format);

    for (int i = 0; format[i] != '\0'; i++) 
	{
        if (format[i] == '%' && format[i + 1] != '\0') 
		{
            i++;
            
            switch (format[i]) 
			{
                case 'c': 
				{
                    c = (char)va_arg(args, int);
                    write(1, &c, 1);
                    caracteres_ecrits++;
                    break;
                }
                case 's': 
				{
                    s = va_arg(args, char *);
                    if (s == NULL) 
						s = "(null)";
                    while (*s) 
					{
						c = *s;
						write(1, &c, 1);
						s++;
                        caracteres_ecrits++;
                    }
                    break;
                }
                case 'd': 
				{
                    d = va_arg(args, int);
                    caracteres_ecrits += imprimer_entier(d);
                    break;
                }
				case 'x':
				{
					x = va_arg(args, unsigned int);
                    caracteres_ecrits += imprimer_hexadecimal(x);
                    break;
				}
                case '%': 
				{
					c = '%';
                    write(1, &c, 1);
                    caracteres_ecrits++;
                    break;
                }
                default: 
				{
					c = '%';
                    write(1, &c, 1);
					c = format[i];
                    write(1, &c, 1);
                    caracteres_ecrits += 2;
                    break;
                }
            }
        } 
		else 
		{
			c = format[i];
            write(1, &c, 1);
            caracteres_ecrits++;
        }
    }

    va_end(args);
    return (caracteres_ecrits);
}

/*
#include <stdlib.h>
int main(int argc, char *argv)
{
    if (argc == 3)
        mini_printf(((const char **)argv)[1], atoi(((const char **)argv)[2]));
    else
        mini_printf("Dommage, pas bon !");
    return 0;
}
*/