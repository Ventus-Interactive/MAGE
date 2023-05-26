
local events = { status = 0 }

function events.init()
    print("- Lua::Events::Init")
    events.status = 1
end

return events
