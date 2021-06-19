#include<curl/curl.h>
#include<stdlib.h>
#include<string.h>
#include<jansson.h>
#include<stdarg.h>
#include"cryptocoins.h"
struct bitcoin_rpc_data{
	const char * method;
	json_t * params;
	char * json_ret;
};

/*thx, Alex Jasmin*/
struct string {
  char *ptr;
  size_t len;
};

static void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s){
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}
json_t * brpc_json_request(struct cryptocoin * c, struct bitcoin_rpc_data * bdata);
json_t * brpc_prepare_params(char * data,...);
void brpc_clear_bdata(struct bitcoin_rpc_data * data);
void brpc_get_json(struct bitcoin_rpc_data * data);



