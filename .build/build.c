
#include "builder.h"


int main(void) {
    RUN("echo \"> building modules...\"")

    BUILD_MOD("assets")
    BUILD_MOD("audio")
    BUILD_MOD("ecs")
    BUILD_MOD("events")
    BUILD_MOD("gui")
    BUILD_MOD("input")
    BUILD_MOD("memory")
    BUILD_MOD("network")
    BUILD_MOD("rendering")
    BUILD_MOD("scripting")
    BUILD_MOD("threading")
    BUILD_MOD("window")

    RUN("echo \"> compiled modules!\"")
}
