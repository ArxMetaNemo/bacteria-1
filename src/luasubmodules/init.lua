local submoduledir = "./luasubmodules/"
json = require "json"
sql = require "luasql.postgres"

bacteria = require (submoduledir .. "bacteria")
bacteria_aes = require (submoduledir .. "bencdec")
bacteria.init("cryptocoins.ini",{'tgst','tdash'})
bacteria.dumpCryptocoins()

function extended (child, parent)
    setmetatable(child,{__index = parent}) 
end


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
--key,iv=bacteria_aes.genKeyIV()

msg="Hello AES_ECB, AES_CBC, ChaCha20! "
local function addChar(ch,time)
	ret=""
	while time > 0 do
		ret = ret .. ch
		time = time - 1
	end
	return ret
end
msg=msg .. addChar("s",666)

b=bacteria_aes.new() --("mysmallkey") -- ("mykey","myiv") 32,16 bytes
print("key:", b:getKey(), "IV: ",b:getIV())
local function checkAllTypes(b,msg)
--	b:encrypt(msg)
--	b:decrypt( b:getAESData_rawEnc() )
--	b:clear()
	for index, data in pairs(AESENCType) do
	    --if data ~= AESENCType["t_chacha20"] then
	    	print(index,data)		
		b:encrypt(msg,data)
		b:decrypt( b:getAESData_rawEnc(),data )
		aesdata_enc,saesdata_enc=b:getAESData_enc()
		b:decrypt(b:getAESData_rawEnc(), data)
		aesdata_dec,saesdata_dec=b:getAESData_dec()
		b:clear()  
		print("Decrypt msg(from local aesdata, from C): ", aesdata_dec)
		print("SetAESData")
		b:setAESData_enc(aesdata_enc,saesdata_enc)
		print("Decrypt")
		b:decrypt(b:getAESData_rawEnc(), data)
		aesdata_dec,saesdata_dec=b:getAESData_dec()
		print("Decrypt msg(from created aesdata, from lua): ", aesdata_dec)
	   -- end

	end

	
end

checkAllTypes(b,"is example message aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafasdjasdjkfsdfjasdfjiaodfasdfjiasdijfasidfadfiaojsdijfoasdfiaojsdfiojasdfijasdfuhasdufhasdiufhasidufashdfiasudhfiasudhfiuasdfihuSOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOFSDJKFASDJFASJDFQJAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")


