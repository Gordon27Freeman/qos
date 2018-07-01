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
