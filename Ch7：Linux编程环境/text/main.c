/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/14/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "10/14/2019 04:25:26 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "crypto.h"

int main(int argc, char *argv[])
{
    char    *message="abcdefghijklmnopq";
    char    ciphertext[64];
    char    plaintext[64];
    
    memset(ciphertext, 0, sizeof(ciphertext));
    if(encrypt(message, ciphertext, sizeof(ciphertext)) < 0)
    {
        printf("encrypt plaintext failure\n");
        return -1;
    }
    printf("encrypt ciphertext: %s\n", ciphertext);
    

    memset(plaintext, 0, sizeof(plaintext));
    if(decrypt(ciphertext, plaintext, sizeof(plaintext)) < 0)
    {                                 
        printf("decrypt plaintext failure\n");
        return -1;                    
    }                                   
    printf("decrypt plaintext: %s\n", plaintext);

    return 0;
}

