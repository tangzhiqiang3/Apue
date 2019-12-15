/********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  crypto.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(10/14/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "10/14/2019 04:06:45 PM"
 *                 
 ********************************************************************************/


#ifndef _CRYPTO_H_
#define _CRYPTO_H_ 

extern int encrypt(char *plaintext, char *ciphertext, int size);

extern int decrypt(char *ciphertext, char *plaintext, int size);

#endif 
