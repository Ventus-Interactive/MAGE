
#include <.common/sdebug.h>
#include <.common/app_main.h>

#include <assets/include.hpp>
#include <audio/include.hpp>
#include <ecs/include.hpp>
#include <events/include.hpp>
#include <gui/include.hpp>
#include <input/include.hpp>
#include <memory/include.hpp>
#include <network/include.hpp>
#include <physics/include.hpp>
#include <rendering/include.hpp>
#include <scripting/include.hpp>
#include <threading/include.hpp>
#include <window/include.hpp>


Test(App_Main)(
    "framework_tester"

    CallTest(mod_assets_init    )
    CallTest(mod_audio_init     )
    CallTest(mod_ecs_init       )
    CallTest(mod_events_init    )
    CallTest(mod_gui_init       )
    CallTest(mod_input_init     )
    CallTest(mod_memory_init    )
    CallTest(mod_network_init   )
    CallTest(mod_physics_init   )
    CallTest(mod_rendering_init )
    CallTest(mod_scripting_init )
    CallTest(mod_threading_init )
    CallTest(mod_window_init    )

)
