#include<curl/curl.h>
#include<stdlib.h>
#include<string.h>
#include<jansson.h>
#include<stdarg.h>
#include"cryptocoins.h"
/*
addmultisigaddress <nrequired> <'["key","key"]'> [account]
addnode <node> <add|remove|onetry>
backupwallet <destination>
createmultisig <nrequired> <'["key","key"]'>
createrawtransaction [{"txid":txid,"vout":n},...] {address:amount,...}
decoderawtransaction <hex string>
dumpprivkey <gostcoinaddress>
encryptwallet <passphrase>
getaccount <gostcoinaddress>
getaccountaddress <account>
getaddednodeinfo <dns> [node]
getaddressesbyaccount <account>
getbalance [account] [minconf=1]
getbestblockhash
getblock <hash> [verbose=true]
getblockcount
getblockhash <index>
getblocktemplate [params]
getconnectioncount
getdifficulty
getgenerate
gethashespersec
getinfo
getmininginfo
getnetworkhashps [blocks] [height]
getnewaddress [account]
getpeerinfo
getrawmempool
getrawtransaction <txid> [verbose=0]
getreceivedbyaccount <account> [minconf=1]
getreceivedbyaddress <gostcoinaddress> [minconf=1]
gettransaction <txid>
gettxout <txid> <n> [includemempool=true]
gettxoutsetinfo
getwork [data]
getworkex [data, coinbase]
help [command]
importprivkey <gostcoinprivkey> [label] [rescan=true]
keypoolrefill
listaccounts [minconf=1]
listaddressgroupings
listlockunspent
listreceivedbyaccount [minconf=1] [includeempty=false]
listreceivedbyaddress [minconf=1] [includeempty=false]
listsinceblock [blockhash] [target-confirmations]
listtransactions [account] [count=10] [from=0]
listunspent [minconf=1] [maxconf=9999999] ["address",...]
lockunspent unlock? [array-of-Objects]
move <fromaccount> <toaccount> <amount> [minconf=1] [comment]
sendfrom <fromaccount> <togostcoinaddress> <amount> [minconf=1] [comment] [comment-to]
sendmany <fromaccount> {address:amount,...} [minconf=1] [comment]
sendrawtransaction <hex string>
sendtoaddress <gostcoinaddress> <amount> [comment] [comment-to]
setaccount <gostcoinaddress> <account>
setgenerate <generate> [genproclimit]
setmininput <amount>
settxfee <amount>
signmessage <gostcoinaddress> <message>
signrawtransaction <hex string> [{"txid":txid,"vout":n,"scriptPubKey":hex,"redeemScript":hex},...] [<privatekey1>,...] [sighashtype="ALL"]
stop
submitblock <hex data> [optional-params-obj]
validateaddress <gostcoinaddress>
verifychain [check level] [num blocks]
verifymessage <gostcoinaddress> <signature> <message>
	
*/
//#define RPCBUFMAX 2056
struct bitcoin_rpc_data{
	const char * method;
	json_t * params;
	char * json_ret;
};

json_t * brpc_prepare_params(char * data,...){
	json_t * ret = json_array();
        va_list ap;
	va_start(ap,data);
	while(data != NULL){
		json_array_append(ret, json_string(data) );
		data = va_arg(ap, char*);
	}
	va_end(ap);
	return ret;
}

void brpc_get_json(struct bitcoin_rpc_data * data){
  char* s = NULL;
  
  json_t *root = json_object();
  json_t * params = data->params;
 
  json_object_set_new( root, "jsonrpc", json_integer( 1 ) );
  json_object_set_new( root, "id", json_string("bacteria") );
  json_object_set_new( root, "method", json_string( data->method ));
  json_object_set_new( root, "params", params );
  
  
  s = json_dumps(root, 0);
  //puts(s);
  data->json_ret = calloc( sizeof(char), strlen(s)+1);
  strcpy(data->json_ret,s);

  json_decref(root);
}
void brpc_clear_bdata(struct bitcoin_rpc_data * data){
	free(data->json_ret);
}

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

static size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
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
/*~~~~~~~~~~~~~~~~~~~~~~~*/

json_t * brpc_json_request(struct cryptocoin * c, struct bitcoin_rpc_data * bdata){
            CURL *curl = curl_easy_init();
	    struct curl_slist *headers = NULL;
	    json_t * ret = NULL;
	    json_error_t error;
	    if (curl) {
		char userpwd[ strlen(c->rpcuser) + strlen(c->rpcpassword) + 2];
		char url[ strlen(c->rpchost) + sizeof("http://") + sizeof("65535") + 2]; 
		sprintf(userpwd, "%s:%s\0", c->rpcuser, c->rpcpassword);
		sprintf(url, "http://%s:%d\0", c->rpchost, c->rpcport);
		
		struct string s;
		init_string(&s);

		const char *data=bdata->json_ret;
		headers = curl_slist_append(headers, "content-type: text/plain;");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		curl_easy_setopt(curl, CURLOPT_USERPWD,
				 userpwd);

		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);


		curl_easy_perform(curl);
		brpc_clear_bdata(bdata);
	        //printf("%s\n", s.ptr);
		ret = json_loads(s.ptr, 0, &error);
    		free(s.ptr);
		s.len=0;
		if(!ret){
			fprintf(stderr, "JSON-RPC; Error decode; Maybe unathorized %d -> %s\n", error.line, error.text);
		}
	    }
	    curl_slist_free_all(headers);
	    curl_easy_cleanup(curl);

	    return ret;
}

static void example(void)
{
	struct bitcoin_rpc_data bdata = {"listaccounts", brpc_prepare_params(NULL)};
	brpc_get_json(&bdata);
	const char * account_name = "ANCMS_Abj1pcMsse";
	struct cryptocoin  c = {false, "gostcoinrpc", "97WDPgQADfazR6pQRdMEjQeDeCSzTwVaMEZU1dGaTmLo",
		 19376, "127.0.0.1", NULL};
	json_t * data = brpc_json_request(&c,&bdata);
	json_t * result = json_object_get(data, "result");

	if(!json_is_object(result))
    	{
		puts("Result not found");
        	json_decref(data);
    //    	exit(1);
    	}else{
		   json_t * ancms_account = json_object_get(result, account_name);
		   float balance = json_number_value(ancms_account);
		   printf("Account balance of %s: %f\n",account_name,balance);
	}
        json_decref(data);
    
}
