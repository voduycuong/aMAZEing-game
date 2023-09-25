#include "string.h"
#include "../uart/uart0.h"

int strlen (char* str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length ++;
    }
    
    return length;
}

void del_line ()
{
    uart_puts("\033[1K");
    uart_puts("\r");
}

int check_str(const char * str1, const char * str2)
{
    char tmp[] = {""};
    for (int i = 0;((str1[i] != ' ') || (str2[i] != '\0')); i++)
    {
            tmp[i] = str1[i];
                if (tmp[i] >= 'A' && tmp[i] <= 'Z')
                {
                    tmp[i] = tmp[i] - 'A' + 'a';
                }
            
            if (tmp[i] != str2[i]) 
            {
                return 0;
            }

    }
    return 1;
}

int strcmp (const char * str1, const char * str2)
{
    char tmp[] = {""};
    
    for (int i = 0;((str1[i] != '\0') || (str2[i] != '\0')); i++)
        {
                tmp[i] = str1[i];
                if (tmp[i] >= 'A' && tmp[i] <= 'Z')
                {
                    tmp[i] = tmp[i] - 'A' + 'a';
                }
                
                if (tmp[i] != str2[i]) 
                {
                    return 0;
                }

        }
        return 1;   
}

char* strcpy(char* pst_str,const char* cpy_str) 
{
    int i;
    for (i = 0; cpy_str[i] != '\0'; i++)
    {
        pst_str[i] = cpy_str[i];
    }
    pst_str[i] = '\0';
    return pst_str; 
}

int is_word_after_word(char *str, char *word1, char *word2) 
{
    int i, j;
    for (i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] == word1[0]) 
        {
            for (j = 1; word1[j] != '\0'; j++) 
            {
                if (str[i + j] != word1[j]) 
                {
                    break;
                }
            }
            if (word1[j] == '\0') 
            {
                i += j;
                while (str[i] == ' ') 
                {
                    i++;
                }
                if (str[i] == word2[0]) {
                    for (j = 1; word2[j] != '\0'; j++) 
                    {
                        if (str[i + j] != word2[j])
                        {
                            break;
                        }
                    }
                    if (word2[j] == '\0') 
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

