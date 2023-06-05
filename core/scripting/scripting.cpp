
#include <_common/sdebug.h>
#include "include.hpp"

#include <ecs/include.hpp>


void mod_scripting_init() {
    PrintD("Mod::scripting_init")
}

void Test_mod_scripting_init() {
    PrintD("Test::scripting_init")
}


namespace MAGE {
namespace Scripting {


bool AppManager::ShouldRun() {
    if (WindowShouldClose()) 
        this->shouldRun = false;
    return this->shouldRun;
}

void AppManager::Start() {
    lua_State* L = this->lua.GetState();

    luaL_dofile(L, "resources/scripts/app.lua");
    //lua_setglobal(L, "ecs");  // expose reference
    lua_settop(L, 0);   // clear the stack

    lua_getglobal(L, "OnStart");
    //lua_getfield(L, -1, "sayhi");  // -1 means stack top.
    lua_pcall(L, 0, 0, 0);            // 0, 0 = #args, #retvals
}

void AppManager::Update() {
    lua_State* L = this->lua.GetState();
    lua_getglobal(L, "OnUpdate");
    //lua_getfield(L, -1, "sayhi");  // -1 means stack top.
    lua_pcall(L, 0, 0, 0);            // 0, 0 = #args, #retvals
}
void AppManager::Render() {
    lua_State* L = this->lua.GetState();
    lua_getglobal(L, "OnRender");
    //lua_getfield(L, -1, "sayhi");  // -1 means stack top.
    lua_pcall(L, 0, 0, 0);            // 0, 0 = #args, #retvals
}

void AppManager::RegisterMethod(std::string name, int (*host_func)(lua_State*)) {
    lua_register(this->lua.GetState(), name.c_str(), host_func);
}

}}
