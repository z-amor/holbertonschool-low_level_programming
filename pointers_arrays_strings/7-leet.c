char *leet(char *s)
{
    char *start = s;
    char letters[] = "aAeEoOtTlL";
    char numbers[] = "4433007711";

    char *1;
    char *n;

    while (*s != '\0')
    {
        l = letters;
        n = numbers;

        
        while (*l != '\0')
        {
            if (*s == *l)
            {
                *s = *n;
            }
            l++;
            n++;
        }
        s++
    }


    return (start);
}
