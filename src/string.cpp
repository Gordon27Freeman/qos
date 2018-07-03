void itoa(char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	if(base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x') divisor = 16;

	do
	{
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);

	*p = 0;

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
	int l1 = strlen(s1), l2 = strlen(s2);
	if (l1 != l2) return 1;
	else
	{
		while (s1[l1] == s2[l1])
		{
			l1++;
			if (l1 >= l2) break;
		}
		if (*s1 == *s2) return 0;
		else return 1;
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
