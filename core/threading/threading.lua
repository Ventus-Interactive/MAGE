
local threading = { status = 0 }

function threading.init()
    print("- Lua::Threading::Init")
    threading.status = 1
end

return threading
