
local network = { status = 0 }

function network.init()
    print("- Lua::Network::Init")
    network.status = 1
end

return network
