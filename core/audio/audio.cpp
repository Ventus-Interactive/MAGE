

#include <_common/sdebug.h>
#include "include.hpp"

void mod_audio_init() {
    PrintD("Mod::audio_init")
}

void Test_mod_audio_init() {
    PrintD("Test::audio_init")
}


namespace MAGE {
namespace Audio {

void LoadModule(lua_State* L) {
    //lua_register(L, "h_world_add_resource", World::lua_AddResource);
    //lua_register(L, "h_world_add_entity"  , World::lua_AddEntity  );

    luaL_dofile(L, "resources/scripts/core/audio.lua");
    lua_setglobal(L, "audio");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

}}
