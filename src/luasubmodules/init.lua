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


-- example in luasubmodules/bencdec.lua
key,iv=bacteria_aes.genKeyIV()
msg="Hello AES_ECB, AES_CBC, ChaCha20!" 
bacteria_aes.SyncCryptoExample( function() 
	return msg
end)




