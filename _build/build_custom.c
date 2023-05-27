
#include "builder.h"

#include <raylib.h>
#include <stdio.h>

#undef BUILD_MOD
void BUILD_MOD(const char* mod) {
    printf("> compiling ... %s\n", mod);

    const char* cmd = TextFormat("%s %s %s %s %s/%s/%s.cpp -o %s/modules/%s.o %s", 
                            CC, STNDRD, COMPILE, INCLUDES, PATH_MODS, mod, mod, PATH_BIN, mod, LIBS);
    //printf("- cmd: %s\n", cmd);
    RUN(cmd)
}

int main(int argc, char* args[]) {
    if (argc <= 1) {
        printf("- No Modules Specified!\n");
        return 0;
    }
    RUN("echo \"> building modules...\"")

    for (int i = 1; i < argc; i++) {
        BUILD_MOD(args[i]);        
    }

    RUN("echo \"> compiled modules!\"")
}
