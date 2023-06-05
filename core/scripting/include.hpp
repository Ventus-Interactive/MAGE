
#ifndef MOD_SCRIPTING
#define MOD_SCRIPTING

extern "C" {
    #include "lib.h"
}


#include <_common/raylib.h>
#include <_common/interfaces.hpp>



namespace MAGE {
namespace Scripting {

/*- Scripting API
  - should mirror native scripting as much as possible
  - IScriptBehavior
    - virtual methods:
      - init      ( IDestroyable )
      - on-enable  ( ITogglable )
      - on-update (  IUpdatable  )
      - on-disable ( ITogglable )
      - destroy   ( IDestroyable )
  - class to wrap LuaState ( IDestroyable )
    - init, get, close
  - class for each module that exposes to LuaEnv
    - loads module's script into LuaState
    - will string_call or method_invoke load/init and clean_up/close
*/

class LuaManager : IDestroyable {
public:
  lua_State* GetState() { return this->state; }
  void Init() { 
    this->state = luaL_newstate(); 
    luaL_openlibs(this->state);
  }
  void Destroy() {
    lua_close(this->state);
  }

protected:
  lua_State* state;
};

class IScriptBehavior : public ITogglable, public IUpdatable, public IDestroyable {
public:
  LuaManager* GetManager() { return &(this->lua); }
  void Init()    { this->lua.Init()   ; }
  void Destroy() { this->lua.Destroy(); }
protected:
  LuaManager lua;
};

class AppManager : public IScriptBehavior {
public:
  void Init() {
    this->lua.Init();
  }
  void Start();//    { this->lua.Init()   ; }
  void Destroy() { this->lua.Destroy(); }
  void Update();
  void Render();

  void RegisterMethod(std::string name, int (*host_func)(lua_State*));

  bool ShouldRun();

private:
  bool shouldRun = true;
};

}}

#endif // !MOD_SCRIPTING
