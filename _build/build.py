
if __name__ == "__main__":

    from subprocess import run as subproc

    dirpath = ".."

    SRC=f"{dirpath}/_build"
    OUT=f"{dirpath}/_bin"
    CC="gcc"

    cmd = ""
    cmd += f"{CC} {SRC}/build_custom.c -o {OUT}/build_custom -lraylib"
    cmd += f" && {CC} {SRC}/build.c -o {OUT}/build"
    
    cmd += f" && {OUT}/build"

    #print(
    subproc(cmd, shell=True)  #.returncode)
