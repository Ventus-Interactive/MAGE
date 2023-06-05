
#include <_common/sdebug.h>
#include "include.hpp"

void mod_input_init() {
    PrintD("Mod::input_init")
}

void Test_mod_input_init() {
    PrintD("Test::input_init")
}


#include <_common/raylib.h>


namespace MAGE {
namespace Input {

static int GetKeyDown(lua_State* L) {
    int code = (int)lua_tonumber(L, 1);
    //printf("- keydown K : %d\n", (int)IsKeyDown(KEY_K));
    //printf("- keydown %c : %d\n", code, (int)IsKeyDown(code));
    lua_pushnumber(L, (int)IsKeyDown(code));

    return 1;
}

void LoadModule(lua_State* L) {
    lua_register(L, "h_core_input_get_key_down", GetKeyDown);

    luaL_dofile(L, "resources/scripts/core/input.lua");
    lua_setglobal(L, "input");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

}}

