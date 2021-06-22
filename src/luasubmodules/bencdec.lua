local baes = { }

-- Test stuff
local function checkEncryption(msg, t)
	print("Type:",t)
--	sencrypted,encrypted=encdec.AESenc(key,iv,msg, t)
--	sdecrypted,decrypted=encdec.AESdec(key,iv,encrypted, sencrypted, t)
	--print("Ecrypted message: ", encrypted, "Size of enc: ", sencrypted)
	--print("Size of enc: ", sencrypted)
--	print("dEcrypted message: ", decrypted, "Size of dec: ", sdecrypted)
	print("Encode")
	aesdata=encdec.AESenc(key,iv,msg,t)
	seccey = encdec.getAESData(aesdata)
	print("Crypted message: ", seccey)
	print("Decode")
	decdata=encdec.AESdec(key,iv,aesdata,t)
	print("plaintext: ",encdec.getAESData(decdata))
	print("Free")
	encdec.freeAESData(aesdata)
	encdec.freeAESData(decdata)
	print("Cycle next")
end
local function checkAllTypes(msg)
	print("New")	
	sleep(1)
	checkEncryption(msg, AESENCType['t_ecb'])
	print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
	checkEncryption(msg, AESENCType['t_cbc'])
	print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
	checkEncryption(msg, AESENCType['t_chacha20'])
	print("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
end
--checkAllTypes("is example message aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafasdjasdjkfsdfjasdfjiaodfasdfjiasdijfasidfadfiaojsdijfoasdfiaojsdfiojasdfijasdfuhasdufhasdiufhasidufashdfiasudhfiasudhfiuasdfihuSOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOFSDJKFASDJFASJDFQJAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")
--checkAllTypes("is example message")
--checkAllTypes("a")
--checkAllTypes("ab")
--checkAllTypes("abc")
--checkAllTypes("abcd")
-- end test stuff

-- GenBytes and later OPENSSL BIGNUM(becase 0.00001 bad zEros=5; sum=1 is good!)

-- AESENCType['t_ecb'], AESENCType['t_cbc'], AESENCType['t_chacha20']
function baes.genRandBytes(len)
	return encdec.genRandBytes(len)
end

function baes.genKeyIV()
	return baes.genRandBytes(32),baes.genRandBytes(16)
end


function baes.decode_raw(key,iv,msg,t)
	aesdata=encdec.createAESData(msg, string.len(msg))
	decdata=encdec.AESdec(key,iv,aesdata,t)
	msg = encdec.getAESData(decdata)
	encdec.freeAESData(aesdata)
	encdec.freeAESData(decdata)
	return msg
end

function baes.encode_raw(key,iv,msg,t)
	aesdata=encdec.AESenc(key,iv,msg,t)
	msg=encdec.getAESData(aesdata)
	encdec.freeAESData(aesdata)
	return msg
end

--msg="Hello AES_ECB,AES_CBC,ChaCha20!"
function baes.encrypt_AEScbc(key,iv,msg)
	return baes.encode_raw(key,iv,msg,AESENCType['t_cbc'])
end
function baes.encrypt_AESecb(key,iv,msg)
	--return baes.encode_raw(key,iv,msg,AESENCType['t_ecb'])
	error("use it better with aesdata... example in luasubmodules/bencdec.lua")
end
function baes.encrypt_AESchacha20(key,iv,msg)
	return baes.encode_raw(key,iv,msg,AESENCType['t_chacha20'])
end

function baes.decrypt_AEScbc(key,iv,msg)
	return baes.decode_raw(key,iv,msg,AESENCType['t_cbc'])
end
function baes.decrypt_AESecb(key,iv,msg)
	--return baes.decode_raw(key,iv,msg,AESENCType['t_ecb'])
	error("use it better with aesdata... example in luasubmodules/bencdec.lua")
end
function baes.decrypt_AESchacha20(key,iv,msg)
	return baes.decode_raw(key,iv,msg,AESENCType['t_chacha20'])
end
-- AES and ChaCha20 / async




-- x25519




return baes
