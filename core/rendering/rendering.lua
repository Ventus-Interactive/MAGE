
-- mod.lua


local rendering = { status = 0, ref_renderers={} }

function rendering.init()
    print("- Lua::Rendering::Init")
    rendering.status = 1
end


function rendering.sayhi()
    print('Why hello from rendering.mod.lua!')
end

function rendering.reg_renderer(name, addr)
    addr= math.floor(addr)
    print('lua::rendering.reg_renderer(name: ' .. name .. ', at: ' .. tostring(addr) .. ')')
    rendering.ref_renderers[name] = {name=name, addr=addr}
end

function rendering.new_renderer(name)
    addr = h_rendering_new_renderer()
    rendering.reg_renderer(name, addr)
end

function rendering.update()
    local ren_len = 0
    for k, v in pairs(rendering.ref_renderers) do
        print(v.name, v.addr)
        ren_len = ren_len + 1
    end
end

function rendering.ref_renderers_count()
    local ren_len = 0
    for _, _ in pairs(rendering.ref_renderers) do
        ren_len = ren_len + 1
    end
    print("lua::rendering.update :: ref_renderers.count: " .. tostring(ren_len))
end


return rendering
