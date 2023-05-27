
#ifndef UTILS_TEST
#define UTILS_TEST

#ifdef FLAG_TESTMODE
    #define Test(x) Test_##x
#else
    #define Test(x) x
#endif

#define CallTest(sig) Test(sig)();

#endif // !UTILS_TEST