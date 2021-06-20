local bacteria = {}

-- cryptocoins
bacteria.coins = {}
local mCryptocoins = {}

function bacteria.reinit(cryptocoins,needcoins) -- not need. deprecated, but may be helpful for some hacks
	mCryptocoins=cryptocoins
	for k,v in ipairs(needcryptocoins) do
		bacteria.coins[v .. ""] = cryptocoins.gettable(mCryptocoins,v)
	end
end

function bacteria.init(ini,needcryptocoins)
	mCryptocoins = cryptocoins.load(ini, needcoins) -- 
	for k,v in ipairs(needcryptocoins) do
		bacteria.coins[v .. ""] = cryptocoins.gettable(mCryptocoins,v)
	end
--print("cryptocoins.clear(t) for clear cryptocoins, but i would to clear from C when program is clear/... and maybe is will be deprecated method. because cryptocoins will be load one time!")

end

for index, data in ipairs(cryptocoins) do
    print(index)

    for key, value in pairs(data) do
        print('\t', key, value)
    end
end
function bacteria.dumpCryptocoins()
list = {}
--print(coins['tgst'])
for k,v in pairs(bacteria.coins) do
	local tmp =    {
       		name = bacteria.coins[k]['name'],
       		host=bacteria.coins[k]['host'],
       		port=bacteria.coins[k]['port'],
       		user=bacteria.coins[k]['user'],
       		password=bacteria.coins[k]['password'],
       		istestnet=bacteria.coins[k]['istestnet']
   		}
	list[k]=tmp
end

for index, data in pairs(list) do
    print(index)

    for key, value in pairs(data) do
        print('\t', key, value)
    end
end


end

-- rpc
function bacteria.doRPCRequest(crypto,method_,params_)
	d=json.encode( { jsonrpc = 1.0, id = "luatest", method = method_, params = params_ })
	r=rpc.request(crypto,d)
	return json.decode(r)
end




return bacteria
