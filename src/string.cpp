void itoa(char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	// set divisor and sign if base is decimal
	if(base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x') divisor = 16;

	// process all digits
	do
	{
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);

	*p = 0;

	// result needs to be reversed
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

int strlen(const char *s)
{
	register int l = 0;
	while(s[l]) l++;
	return l;
}

int strcmp(const char *s1, const char *s2)
{
	for(int i = 0; ; i++)
    {
        if(s1[i] != s2[i]) return s1[i] < s2[i] ? -1 : 1;
        if(s1[i] == '\0') return 0;
	}
}

void strcpy(char *dest, const char *src)
{
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = 0;
}
