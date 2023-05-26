
local networking = { status = 0 }

function networking.init()
    print("- Lua::Networking::Init")
    networking.status = 1
end

return networking
