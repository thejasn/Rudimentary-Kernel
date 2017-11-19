#include "system.h"

unsigned short * memcpy(unsigned short * dest, const unsigned short *src,int count)
{
    int i;
    for(i=0;i<count;i++)
        dest[i]=src[i];
    return dest;
}

int  strlen(const char * str)
{
    int  len =0;
    while(str[len])
        len++;
    return len;
}

unsigned short int str_compare(unsigned char * arg1,unsigned char * arg2)
{
    int i,flag=0;
    if(strlen(arg1) != strlen(arg2))
        return 0;
    for(i=0;i<strlen(arg1);i++)
    {
        if(arg1[i]!=arg2[i])
            return flag;
    }
    if(i==strlen(arg1)) flag=1;
    return flag;
    
}
unsigned char * str_cat(unsigned char * arg1,unsigned char * arg2)
{
    int i,k=0;
    for(i = strlen(arg1);k<strlen(arg2);i++,k++)
        arg1[i]=arg2[k];
    return arg1;
 
}
unsigned char * str_add(unsigned char * arg1,unsigned char * arg2)
{
    int i,k=0;
    char *new_str;
    for(i=0;i<strlen(arg1);i++)
        new_str[i]=arg1[i];
    for(i = strlen(arg1);k<strlen(arg2);i++,k++)
        new_str[i]=arg2[k];
    return new_str;
 
}
//unsigned short int str_copy(unsigned char * src,unsigned char * dest)
char * generate_prompt()
{
    //str_cat(CURRENT_PATH,PROMPT);
    //terminal_writestring(CURRENT_PATH);
    return ((CURRENT_PATH));
}
unsigned short * memsetw(unsigned short * dest, unsigned short val,int count)
{
    int i;
    for(i=0;i<count;i++)
        dest[i]=val;
    return dest;
}
unsigned char * strset( char * dest, char val,int count)
{
    int i;
    for(i=0;i<count;i++)
        dest[i]=val;
    return dest;
}
int  str_substr_end(unsigned char * str,unsigned char * substr)
{
    int i,j;
    for(i=0;i<strlen(str);i++)
    {
        if(str[i]==substr[0])
        {
            for(j=0;j<strlen(substr);j++)
            {
                if(str[i+j] != substr[j])
                    break;
            }
            if(j==strlen(substr))
                return j;
        }
    }
    return -1;
}
void str_copy(char src[],char dest[])
{
    int i;
    strset(dest,'\0',strlen(dest));
    for(i=0;i<strlen(src);i++)
    {
        dest[i] = src[i];
    }
}
void ret_convertor(int i)
{
    if(i==0)
        terminal_writestring("0\n");
    else if(i==1)
        terminal_writestring("1\n");
    else if(i==-1)
        terminal_writestring("-1\n");

}
void clear_str(char * str,char a,int len)
{
    int i;
    for(i=0;i<len;i++)
        str[i] = a;
}