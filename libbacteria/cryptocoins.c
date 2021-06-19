#include"cryptocoins.h"

struct cryptocoin * init_cryptocoins(const char * pathToIniFile){
	#define GET_SEC(def) sprintf(tmp_buf, "%s:%s", sec_name, "" #def); 	
		/*
	#define GET_VAL_DEFNOSTR(def,dev_val,type,command)\
		GET_SEC(def);\
		const type pre_##def = command(init_file, tmp_buf, dev_val);

	#define GET_VAL_DEFSTR(def,dev_val,type,command)\
		GET_SEC(def);\
		const type pre_##def = command(init_file, tmp_buf, "" #dev_val);
*/
	#define GETSEC_INT(def,dev_val) \
		 GET_SEC(def);\
		 const int pre_##def = iniparser_getint(ini_file,tmp_buf, dev_val);

	#define GETSEC_STRING(def,dev_val) \
		 GET_SEC(def);\
		 const char* pre_##def = iniparser_getstring(ini_file,tmp_buf, "" #dev_val);

	#define GETSEC_BOOL(def) \
		 GET_SEC(def);\
		 const bool pre_##def = iniparser_getboolean(ini_file,tmp_buf, 0);

/*
 cryptocoins[i].rpcuser = malloc (sizeof(char) * strlen(rpcuser)+1);
		 strcpy(cryptocoins[i].rpcuser,rpcuser);
		 cryptocoins[i].rpcuser[strlen(rpcuser)]=0;

#define SET_VALUE(what) {\
		cryptocoins[i].#what = malloc (sizeof(char) * strlen(#what)+1);\
		 strcpy(cryptocoins[i].#what,#what);\
		 cryptocoins[i].#what[strlen(#what)]=0;\
	}	
*/
#define crypt cryptocoins[i]
#define SET_VALUE_string(what)\
	cryptocoins[i].what = calloc( sizeof(char) , strlen(pre_##what)+1);\
	strcpy(crypt.what, pre_##what);


	dictionary * ini_file = iniparser_load(pathToIniFile);
	if(ini_file == NULL) return NULL;
	int count_cryptocoins = iniparser_getnsec(ini_file);
	struct cryptocoin * cryptocoins = calloc( sizeof(struct cryptocoin) , count_cryptocoins);
	if(cryptocoins == NULL) {
		perror("Access memory");
		exit(EXIT_FAILURE);
	}

	char * sec_name;
	char tmp_buf[1024];
	bzero(tmp_buf,sizeof(tmp_buf));

	for(int i = count_cryptocoins;i--;){
		 sec_name =(char*) iniparser_getsecname(ini_file, i);
		 const char * pre_cryptocoin_name = sec_name;
		 SET_VALUE_string(cryptocoin_name);

		 //printf("load %s\n", sec_name);
		 
	         GETSEC_STRING(rpcpassword, nopass);
	         GETSEC_STRING(rpcuser, user);
	         GETSEC_STRING(rpchost, localhost);
		 GETSEC_INT(rpcport, pre_rpcport);
		 GETSEC_BOOL(testnet);

		 SET_VALUE_string(rpcuser);
		 SET_VALUE_string(rpcuser);
		 SET_VALUE_string(rpchost);
		 SET_VALUE_string(rpcpassword);
		 cryptocoins[i].testnet=pre_testnet;
		 cryptocoins[i].rpcport=pre_rpcport;

		 //printf("%s:%d %s:%s testnet:%s\n",crypt.rpchost, crypt.rpcport, crypt.rpcuser, 
		 //	crypt.rpcpassword, crypt.testnet? "is testnet" : "is not testnet");

		 sprintf(tmp_buf, "%s:%s", sec_name, "testnet");
		 cryptocoins[i].testnet= iniparser_getboolean(ini_file, tmp_buf, 0);
	}
	#undef GET_SEC
	#undef GETSEC_INT
	#undef GETSEC_STRING
	#undef GETSEC_BOOL
	#undef GETSEC_STRING
	#undef GETSEC_INT
	#undef crypt
	iniparser_freedict(ini_file);
	return cryptocoins;
};

void dump_cryptocoins(struct cryptocoin* cryptocoins){
	for(unsigned char i = 0; cryptocoins[i].rpchost != NULL && cryptocoins[i].rpcport != 0;i++){
		printf("Cryptocoin - %s\n",cryptocoins[i].cryptocoin_name);
		printf("%s:%d %s:%s testnet:%s\n",cryptocoins[i].rpchost, cryptocoins[i].rpcport, cryptocoins[i].rpcuser, 
			cryptocoins[i].rpcpassword, cryptocoins[i].testnet? "is testnet" : "is not testnet");	
	}
}

void clear_cryptocoins(struct cryptocoin * cryptocoins){
	for(unsigned char i = 0; cryptocoins[i].rpchost != NULL && cryptocoins[i].rpcport != 0;i++){
		free(cryptocoins[i].cryptocoin_name);
		free(cryptocoins[i].rpchost);
		free(cryptocoins[i].rpcuser);
		free(cryptocoins[i].rpcpassword);
		free(cryptocoins[i].rpchost);
		cryptocoins[i].testnet=false;
		cryptocoins[i].rpchost=0;
	}
	free(cryptocoins);
}
