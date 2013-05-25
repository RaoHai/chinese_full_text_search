#include "cstlib.h"



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
char * trim(char * ptr)
{
    int start,end,i;
    if (ptr)
    {
        for(start=0; isspace(ptr[start]); start++)
            ;
        for(end=strlen(ptr)-1; isspace(ptr[end]); end--)
            ;
        for(i=start; i<=end; i++)
            ptr[i-start]=ptr[i];
        ptr[end-start+1]='\0';
        return (ptr);
    }
    else
        return NULL;
}

//前段时间看到je上一个c++实现split函数，恰好项目需要写了一个c语言的split方法，欢迎大家提出意见，下面是代码

char** cSplit(char *string, char spliter)
{
    int num = 0;

    //计算数量
    int i = 0;
    int size = strlen(string);
    for(; i < size; i++)
    {
        //
        if((string[i] != spliter) && (string[i + 1] == spliter || string[i + 1] == '\0')) num++;
    }
    //分配空间
    char **cStrings = (char**)malloc(sizeof(char*) * (num + 1));
    int j = 0;
    for(; j <= num; j++)
    {
        //初始化指针数组
        cStrings[j] = 0;
    }
    //保存分割后字符串
    char *buffer = (char*)malloc(sizeof(char) * (size + 1));
    int l = 0;
    for(; l < size + 1; l++)
    {
        buffer[l] = 0;
    }
    int counter = 0;
    int k = 0;
    for(; k < size; k++)
    {
        if(string[k] != spliter)
        {
            int i = 0;
            int j = k;
            while(string[j] != spliter && string[j] != '\0')
            {
                buffer[i++] = string[j++];
            }
            buffer[i] = '\0';
            cStrings[counter] = (char*)malloc(sizeof(char) * i);
            strcpy(cStrings[counter],buffer);
            counter ++;
            k = k + i;
        }
    }
    free(buffer);
    return cStrings;
}

/**
*调用此方法释放cSplit分配的内存空间
*/
void freeStringsMemory(char **strings)
{
	//
	int k = 0;
	for(; ; k++)
	{
		//
		if(strings[k] != 0) free(strings[k]);
		else break;
	}
	free(strings);
}

void k_log(const char* fmt,...){
    va_list ap ;
    va_start( ap , fmt );
    #ifdef LOG_TO_CONSOLE
    printf( fmt , ap );
    #endif
    #ifdef LOG_TO_FILE
    printf("FILE:%d\n",tp);
    if(!tp)  tp = fopen("out.txt","a");
    fprintf(tp,fmt,ap);

    #endif
    va_end( ap );
}
