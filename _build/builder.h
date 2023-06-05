
#ifndef BUILDER_H
#define BUILDER_H


#include <stdlib.h>


#define CMD(m...) "" m ""
#define RUN(cmd) system((cmd));


#define PATH_BIN "../_bin"
#define PATH_MODS "../core"
#define CC " g++ "
#define STNDRD " -std=c++17 "
#define COMPILE " -c "
#define INCLUDES " -pthread -I../core -I../core/_common   -I. -I../core "
#define LIBS " -lraylib -llua -lm -ldl -lpthread -lwinmm "

/// windows
//#define INCLUDES " -pthread -I../core -I../core/_common  -IC:\\raylib\\src -LC:\\raylib\\src -I. -I../core "
//#define LIBS " -lraylib -lopengl32 -lgdi32 -llua -lm -ldl -lpthread -lwinmm -mwindows "
//#define CC " C:\\raylib\\w64devkit\\bin/g++.exe C:\\raylib\\src\\raylib.rc.data "

#define MOD(name) PATH_BIN "/modules/" name ".o " 

#define BUILD_MOD(mod) RUN(CMD(" echo \"> compiling ... " mod "\" && " CC STNDRD COMPILE INCLUDES PATH_MODS "/" mod "/" mod ".cpp -o " PATH_BIN "/modules/" mod ".o" LIBS))


#endif // !BUILDER_H
