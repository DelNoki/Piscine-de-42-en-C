char *ft_strcpy(char *src, char *dest)
{
    int i = 0;
    if (!src)
        return ((void *)0);
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return (dest);
}