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

#define HANDLERRSSL() { ERR_print_errors_fp(stderr);return -1; }


#define CHECKTYPE(whose,algo)\
    if( (enctype& t_##whose ) == t_##whose ){\
	puts("t_"#whose );\
	ciphertext_len = algo##_encrypt (plaintext, strlen ((char *)plaintext), key, iv,\
                              ciphertext);\
	printf("Ciphertext is:\n");\
        BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);\
    	decryptedtext_len = algo##_decrypt(ciphertext, ciphertext_len, key, iv,\
                                decryptedtext);\
    	decryptedtext[decryptedtext_len] = '\0';\
  	printf("Decrypted text is:\n");\
  	printf("%s\n", decryptedtext);\
    }

typedef const EVP_CIPHER *(*aes_enctype)(void);



int AES_any_decrypt(aes_enctype enc , unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);
int AES_any_encrypt(aes_enctype enc,unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext);

INITAESALGO(AES_256_cbc, EVP_aes_256_cbc);
INITAESALGO(AES_256_ecb, EVP_aes_256_ecb);
INITAESALGO(chacha20_poly1305,EVP_chacha20_poly1305);


void generate_rand_bytes(size_t len, unsigned char * key);

enum ENCType{
 	t_cbc=1<<1,
	t_ecb=1<<2, // for small security operations/individual information
	t_chacha20=1<<3
};


