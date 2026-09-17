int ft_atoi(char *nb)
{
	int i = 0;
	int tot = 0;
    int minus = 0;
    while (nb[i] == ' ')
        i++;
    while (nb[i] == '+' || nb[i] == '-')
    {
        if (nb[i] == '-')
            minus++;
        i++;
    }
	while(nb[i] >= '0' && nb[i] <= '9')
	{
		tot = tot * 10 + nb[i] - '0';
		i++;
	}
    if (minus % 2 != 0)
        tot *= -1;
	return (tot);
}