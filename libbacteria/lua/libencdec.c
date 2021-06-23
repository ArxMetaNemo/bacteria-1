#include "libencdec.h"

#define LUA_ENUM(L, val)                                                       \
  lua_pushliteral(L, #val "");                                                 \
  lua_pushnumber(L, val);                                                      \
  lua_settable(L, -3)

int luaopen_encdec(lua_State *L) {
  lua_newtable(L);
  LUA_ENUM(L, t_ecb);
  LUA_ENUM(L, t_cbc);
  LUA_ENUM(L, t_chacha20);
  lua_setglobal(L, "AESENCType");
  luaL_openlib(L, "encdec", encdeclib, 0);
  return 1;
}

int lua_genRandBytes(lua_State *L) {
  int len = (int)luaL_checknumber(L, 1);
  if (len <= 0) {
    luaL_error(L, "genRandBytes. bytes will be more than 0.");
  }
  char bytes[len + 1];
  generate_rand_bytes(len, bytes);
  bytes[len] = '0';
  lua_pushstring(L, bytes);
  return 1;
}

#define INITENCTYPE(prefix, algo)                                              \
  if ((type & t_##prefix) == t_##prefix) {                                     \
    ciphertext_len = algo##_encrypt(plaintext, size_msg, key, iv, ciphertext); \
  }
int lua_AESenc(lua_State *L) {
  char *key = (char *)luaL_checkstring(L, 1);
  char *iv = (char *)luaL_checkstring(L, 2);
  unsigned char *plaintext = (unsigned char *)luaL_checkstring(L, 3);
  int type = (int)luaL_checknumber(L, 4);

  int ciphertext_len;
  int size_msg = strlen(plaintext);
  if (size_msg == 0)
    return 0;
  unsigned char *ciphertext = (unsigned char *)malloc(size_msg * sizeof(char));
  if (ciphertext == NULL)
    return 0;
#ifdef DEBUGAES
  printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ENC~~~~~~~~~~~~~~~~~~~~~~~~~"
         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif

  INITENCTYPE(cbc, AES_256_cbc)
  else INITENCTYPE(ecb, AES_256_ecb) else INITENCTYPE(
      chacha20, chacha20_poly1305) else return 0;
  ciphertext[ciphertext_len] = '\0';

#ifdef DEBUGAES
  for (unsigned int i = 0; i < ciphertext_len; i++) {
    printf("%d ", ciphertext[i]);
  }
  printf("\n\n\n");
  printf("\n\n\n");
  printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif
  if (ciphertext_len < 0) {
    fprintf(stderr, "Error openssl. see logs.\n");
    return 0;
  }
  size_t nbytes = sizeof(struct lua_AESData) +
                  (ciphertext_len - 1) * sizeof(size_t) + sizeof(char *);
  struct lua_AESData *ret = (struct lua_AESData *)lua_newuserdata(L, nbytes);
  ret->size = ciphertext_len;
  ret->data = ciphertext;
  return 1;
  //      lua_pushstring(L, ciphertext);
  //	free(ciphertext);
  //	return 2;
}

#define INITDECTYPE(prefix, algo)                                              \
  if ((type & t_##prefix) == t_##prefix) {                                     \
    plaintext_len = algo##_decrypt(in->data, in->size, key, iv, plaintext);    \
  }
int lua_AESdec(lua_State *L) {

#ifdef DEBUGAES
  printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DEC~~~~~~~~~~~~~~~~~~~~~~~~~"
         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif

  char *key = (char *)luaL_checkstring(L, 1);
  char *iv = (char *)luaL_checkstring(L, 2);
  struct lua_AESData *in = (struct lua_AESData *)lua_touserdata(L, 3);
  int type = (int)luaL_checknumber(L, 4);

  if (in->data == 0 || in->size == 0)
    return 0;

  int plaintext_len;
  char *plaintext = (char *)malloc(in->size * sizeof(char));
  if (plaintext == NULL)
    return 0;

#ifdef DEBUGAES
  for (unsigned int i = 0; i < in->size; i++) {
    printf("%d ", in->data[i]);
  }
  printf("\n\n\n");
  printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif

  INITDECTYPE(cbc, AES_256_cbc)
  else INITDECTYPE(ecb, AES_256_ecb) else INITDECTYPE(
      chacha20, chacha20_poly1305) else return 0;

  plaintext[plaintext_len] = '\0';
  if (plaintext_len < 0) {
    fprintf(stderr, "Error openssl. see logs.\n");
    return 0;
  }

  /*if(in->data!=NULL){
          free(in->data);
  }
  in->size=plaintext_len;
//	lua_settop(L,0);
  in->data=plaintext;
//        lua_pushnumber(L, plaintext_len);
  lua_pushboolean(L,1);*/
  size_t nbytes = sizeof(struct lua_AESData) +
                  (plaintext_len - 1) * sizeof(size_t) + sizeof(char *);
  struct lua_AESData *ret = (struct lua_AESData *)lua_newuserdata(L, nbytes);
  ret->size = plaintext_len;
  ret->data = plaintext;
  return 1;
  //        lua_pushstring(L, plaintext);
  //	free(plaintext);
  //	return 2;
}

INITLUAFUNC(createAESData) {
  unsigned char *data = (unsigned char *)luaL_checkstring(L, 1);
  long long size = (long long)luaL_checknumber(L, 2);
  size_t nbytes = sizeof(struct lua_AESData) + (size - 1) * sizeof(size_t) +
                  sizeof(unsigned char *);
  struct lua_AESData *ret = (struct lua_AESData *)lua_newuserdata(L, nbytes);
  ret->size = size;
  ret->data = (unsigned char *)malloc(size * sizeof(unsigned char));
  strncpy(ret->data, data, size);
  return 1;
}

INITLUAFUNC(getAESData) {
  struct lua_AESData *in = (struct lua_AESData *)lua_touserdata(L, 1);
  if (in == NULL || in->data == 0 || in->size == 0)
    return 0;
  lua_pushstring(L, in->data);
  return 1;
}

INITLUAFUNC(getAESData_len) {
  struct lua_AESData *in = (struct lua_AESData *)lua_touserdata(L, 1);
  if (in == NULL || in->data == 0 || in->size == 0)
    return 0;
  lua_pushnumber(L, in->size);
  return 1;
}

int lua_freeAESData(lua_State *L) {
  struct lua_AESData *in = (struct lua_AESData *)lua_touserdata(L, 1);
  if (in == 0)
    luaL_error(L, "aesdata is broken");

  if (in->data != 0 || in->size != 0) {
    free(in->data); // in->size=0;in->data=0;
    lua_pushboolean(L, 1);
    return 1;
  }
  lua_pushboolean(L, 0);
  return 1;
}

// x25519 and MAYBE rsa(mamonth...)aa support LATTER(not😠️need)

int lua_getKeyPair(lua_State *L) {}

int lua_freeKeyPair(lua_State *L) {}

int lua_freeSharedKey(lua_State *L) {}

int lua_createKeyPair(lua_State *L) {}

int lua_getSharedKey(lua_State *L) {}
