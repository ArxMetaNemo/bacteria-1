local submoduledir = "./luasubmodules/"
json = require "json"
sql = require "luasql.postgres"

bacteria = require (submoduledir .. "bacteria")
bacteria_aes = require (submoduledir .. "bencdec")
bacteria.init("cryptocoins.ini",{'tgst','tdash'})
bacteria.dumpCryptocoins()


function sleep (a) 
    local sec = tonumber(os.clock() + a); 
    while (os.clock() < sec) do 
    end 
end


local function check_leak_memory(fcoin,scoin) 
		tmp=bacteria.doRPCRequest(fcoin, "getbalance", {"*",6})
		tmp1=bacteria.doRPCRequest(scoin, "getbalance", {"*",6})

		local iter=65535 -- check to memory leak --
		--while 1 == 1 do
		while iter > 65535 do
			iter=iter-1
			bacteria.doRPCRequest(tgst, "getbalance", {"*",6})
			bacteria.doRPCRequest(sgst, "getbalance", {"*",6})
		end
		print(tmp1['result'])
		--print(tmp)
		print(tmp['result'])
end 
--tgst=cryptocoins.gettable(t,'tgst')
--tdash=cryptocoins.gettable(t,'tdash')
tgst = bacteria.coins['tgst']
tdash = bacteria.coins['tdash']
check_leak_memory(tgst,tdash)

key,iv=bacteria_aes.genKeyIV()
msg="Hello AES_CBC,ChaCha20!"
enc=bacteria_aes.encode_raw(key,iv,msg, AESENCType['t_chacha20'])
print("Encrypted msg: ", enc)
dec=bacteria_aes.decode_raw(key,iv,enc,AESENCType['t_chacha20'])
print("Decrypted msg: ", dec)
print("\n\n\n\n\n\n")
print("AES_CBC")
cbc=bacteria_aes.encrypt_AEScbc(key,iv,"hewwo")
--ecb=bacteria_aes.encrypt_AESecb(key,iv,"hewwo")
print("CBC:",cbc)
print("\n\n\n\n\n\n")
print("Decrypted")
cbc=bacteria_aes.decrypt_AEScbc(key,iv,cbc)
--ecb=bacteria_aes.decrypt_AESecb(key,iv,cbc)
print("CBC:",cbc)




