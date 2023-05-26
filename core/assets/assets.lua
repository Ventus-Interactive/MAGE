
local assets = { status = 0 }

function assets.init()
    print("- Lua::Assets::Init")
    assets.status = 1
end

return assets
