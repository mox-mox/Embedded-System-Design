
char *
strncpy(char *d, const char *s, size_t n)
{
    char	*rval = d;

    while(n != 0 && *s) n--, *d++ = *s++;
    while(n != 0) n--, *d++ = 0;
    return(rval);
}

