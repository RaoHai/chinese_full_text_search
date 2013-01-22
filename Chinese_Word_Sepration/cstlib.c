#include "cstlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

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
