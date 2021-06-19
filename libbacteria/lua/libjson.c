#include"libjson.h"
int lua_rpc_requst(lua_State * L){//struct cryptocoin * c, struct bitcoin_rpc_data * bdata) 
	    if (!lua_istable(L, 1)){
			luaL_error(L,"`table` error");
			return 0;
	    }
		
	    lua_pushstring(L, "rpcport");
	    lua_gettable(L, -2);
            if (!lua_isnumber(L, -1)){
		luaL_error(L, "invalid rpcport");
	    }
	    uint port = (int)lua_tonumber(L, -1);
	    lua_pop(L, 1);

            lua_pushstring(L, "istestnet");
	    lua_gettable(L, -2);
            if (!lua_isnumber(L, -1)){
		luaL_error(L, "invalid rpcport");
	    }
	    bool istestnet = (int)lua_tonumber(L, -1);
	    lua_pop(L, 1);

	    struct cryptocoin  a = {false, "gostcoinrpc", "97WDPgQADfazR6pQRdMEjQeDeCSzTwVaMEZU1dGaTmLo",
		 19376, "127.0.0.1", NULL};
	    struct cryptocoin * c = &a;
		
//	    luaL_argcheck(L, c != NULL, 1, "`cryptocoin data' expected");

            CURL *curl = curl_easy_init();
	    struct curl_slist *headers = NULL;
	   // json_t * ret = NULL;
	   // json_error_t error;
	    if (curl) {
		char userpwd[ strlen(c->rpcuser) + strlen(c->rpcpassword) + 2];
		char url[ strlen(c->rpchost) + sizeof("http://") + sizeof("65535") + 2]; 
		sprintf(userpwd, "%s:%s\0", c->rpcuser, c->rpcpassword);
		sprintf(url, "http://%s:%d\0", c->rpchost, c->rpcport);
		
		struct string s;
		init_string(&s);

		const char *data="";//bdata->json_ret;
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
		//brpc_clear_bdata(bdata);
	        //printf("%s\n", s.ptr);
		//ret = json_loads(s.ptr, 0, &error);
    		free(s.ptr);
		//s.len=0;
		//if(!ret){
		//	//fprintf(stderr, "JSON-RPC; Error decode; Maybe unathorized %d -> %s\n", error.line, error.text);
		//}
	    }
	    curl_slist_free_all(headers);
	    curl_easy_cleanup(curl);

	    return 0;
}

int luaopen_rpc (lua_State *L){
      luaL_openlib(L, "rpc", rpclib, 0);
      return 1;

}
