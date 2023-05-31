
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
#define INCLUDES " -I. -I../core "
#define LIBS " -lraylib -llua -lm -ldl "


#define MOD(name) PATH_BIN "/modules/" name ".o " 

#define BUILD_MOD(mod) RUN(CMD(" echo \"> compiling ... " mod "\" && " CC STNDRD COMPILE INCLUDES PATH_MODS "/" mod "/" mod ".cpp -o " PATH_BIN "/modules/" mod ".o" LIBS))


#endif // !BUILDER_H
