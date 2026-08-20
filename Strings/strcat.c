char *ft_strcat(char *src, char *dest)
{
    int i = 0;
    int j = 0;
    if (!src)
        return (dest);
    while (dest[i])
        i++;
    while (src[j])
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = 0;
    return (dest);
}