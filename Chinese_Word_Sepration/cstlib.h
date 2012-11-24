/*
 * 一些需要用到的库函数
 */
#define LOWER(c)            (unsigned char)(c | 0x20)


char* L(char* s)
{
	int i=0;
	while(s[i])
	{
		s[i] = LOWER(s[i]);
		i++;
	}
	return s;

}