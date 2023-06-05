
if __name__ == "__main__":

    from subprocess import run as subproc

    dirpath = "../MAGE"

    
    SRC=f"{dirpath}/_build"
    OUT=f"{dirpath}/_bin"

    CC="gcc"
    FLAGS="-std=c99 -Wall"
    LIBS="-lraylib "
    INCLUDES=f"-I{dirpath}/core/_common"
    

    ## windows
    #RAYLIB_INCLUDE_DIR='C:\\raylib\\src'
    #RAYLIB_LIB_DIR='C:\\raylib\\src'
    #RAYLIB_RES_FILE='C:\\raylib\\src\\raylib.rc.data'
    #COMPILER_DIR='C:\\raylib\\w64devkit\\bin'
    #LIBS += " -lopengl32 -lgdi32 -lwinmm -mwindows"
    #CC=f"{COMPILER_DIR}/gcc.exe"
    #INCLUDES=f"{RAYLIB_RES_FILE} -I{dirpath}/core/_common -I{RAYLIB_INCLUDE_DIR} -L{RAYLIB_LIB_DIR}"
    ###

    
    cmd = ""
    cmd += f"{CC} {FLAGS} {INCLUDES} {SRC}/build_custom.c -o {OUT}/build_custom {LIBS}"
    cmd += f" && {CC} {FLAGS} {INCLUDES} {SRC}/build.c -o {OUT}/build {LIBS}"
    
    #cmd += f" && \"{OUT}/build.exe\""  # windows
    cmd += f" && {OUT}/build"

    #print(
    subproc(cmd, shell=True)  #.returncode)
