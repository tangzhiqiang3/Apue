/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  crypto.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/14/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "10/14/2019 04:05:47 PM"
 *                 
 ********************************************************************************/

#include <string.h>

int encrypt(char *plaintext, char *ciphertext, int size )
{
    int     i;
    if(size < strlen(plaintext)) 
    {
        return -1;   
    }
    for (i = 0; i < strlen(plaintext); ++i) 
    {
        ciphertext[i]=plaintext[i]+3;
    }
    return 0;
}

int decrypt(char *ciphertext, char *plaintext, int size )
{
    int     i;
    if(size < strlen(ciphertext))
    {             
        return -1;
    }
    
    for (i = 0; i < strlen(ciphertext); ++i)
    {
        plaintext[i]=ciphertext[i]-3;
        
    }
    return 0;

}


