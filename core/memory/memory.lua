
local memory = { status = 0 }

function memory.init()
    print("- Lua::Memory::Init")
    memory.status = 1
end

return memory
