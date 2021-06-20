#pragma once

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#define CREATEAESALGO(prename, algo) \
int prename ## _decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,\
            unsigned char *iv, unsigned char *plaintext){\
	return AES_any_decrypt(algo, ciphertext, ciphertext_len, key, iv, plaintext);\
}\
int prename ## _encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,\
            unsigned char *iv, unsigned char *ciphertext){\
	return AES_any_encrypt(algo, plaintext, plaintext_len, key, iv, ciphertext );\
}

#define INITAESALGO(prename,algo)\
int prename ## _encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,\
            unsigned char *iv, unsigned char *ciphertext);\
int prename ## _decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,\
            unsigned char *iv, unsigned char *plaintext);

#define HANDLERRSSL() { ERR_print_errors_fp(stderr);abort(); }
typedef const EVP_CIPHER *(*aes_enctype)(void);



int AES_any_decrypt(aes_enctype enc , unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);
int AES_any_encrypt(aes_enctype enc,unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);

INITAESALGO(AES_256_cbc, EVP_aes_256_cbc);
INITAESALGO(AES_256_ecb, EVP_aes_256_ecb);

enum ENCType{
 	t_cbc=1<<1,
	t_ecb=1<<2 // for small security operations/individual information
};


