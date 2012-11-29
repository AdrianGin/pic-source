





void replace_char(char *s, char find, char replace)
{
	while (*s != 0)
	{
		if (*s == find)
		*s = replace;
		s++;
	}
}