#pragma once
#include<openssl/crypto.h>
#include<openssl/evp.h>
#include<stdio.h>
#include<string.h>
#define LENKEY 32
struct keysPair{
	EVP_PKEY * privKey;	//, *pubKey ;
	unsigned char pubKey[LENKEY+1];
	EVP_PKEY_CTX * pKeyCtx;//just ctx
};

struct keysPair createKeyPair(const uint8_t * priv, const uint8_t * pub);
void freeKeyPair(struct keysPair * pair);
void freeSharedKeys(uint8_t * w, ...);
void freeSharedKey(uint8_t * w);
struct keysPair generateKeyPair(void);
uint8_t * getSharedKey( struct keysPair * pair, const uint8_t * pubPeer, size_t * skeylen );
void getRawPubKey(EVP_PKEY * privKey, uint8_t * pub);
void getRawPrivKey(EVP_PKEY * privKey, uint8_t * priv);
