json = require "json"
t=cryptocoins.load("cryptocoins.ini")
tgst=cryptocoins.gettable(t,'tgst')
tdash=cryptocoins.gettable(t,'tdash')

d=json.encode( { jsonrpc = 1.0, id = "curltest", method = "getbalance", params = {"*",6} })
--print(d)
r=rpc.request(tgst,d)
print(r)
r=rpc.request(tgst,d)
print(r)
r=rpc.request(tgst,d)
print(r)
r=rpc.request(tgst,d)
print(r)

cryptocoins = {
   {
       name = tgst['name'],
       host=tgst['host'],
       port=tgst['port'],
       user=tgst['user'],
       password=tgst['password'],
       istestnet=tgst['istestnet']
   },
    {
       name = tdash['name'],
       host=tdash['host'],
       port=tdash['port'],
       user=tdash['user'],
       password=tdash['password'],
       istestnet=tdash['istestnet']
   }
}

for index, data in ipairs(cryptocoins) do
    print(index)

    for key, value in pairs(data) do
        print('\t', key, value)
    end
end

--('{"jsonrpc": "1.0", "id":"curltest", "method": "getbalance", "params": ["*", 6] }')
--print("cryptocoins.clear(t) for clear cryptocoins, but i would to clear from C when program is clear/... and maybe is will be deprecated method. because cryptocoins will be load one time!")
-- print(tgst['host'])
-- print(tgst['...'])