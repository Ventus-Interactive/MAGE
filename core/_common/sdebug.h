
#ifndef COMMON_SDEBUG
#define COMMON_SDEBUG


#include <stdio.h>

#define PrintD(msg) printf("- #%d :: %s\n", __LINE__, msg);
#define PrintF(msg, v...) printf("- #%d :: %s\n", __LINE__, msg, v);


#ifdef FLAG_TESTMODE
    #include <iostream>
    #include <fstream>

    #define Log(msg) PrintD(msg)
    #define PATH_LOG_FILE "log/output.txt"
    #define LogFile(msg) { Log("- logging to file...") ofstream f; f.open(PATH_LOG_FILE); f << msg << "\n"; f.close(); }
#else
    #define Log(msg) /// disabled due to non-test mode
    #define LogFile(msg) /// disabled due to non-test mode
#endif


#endif
