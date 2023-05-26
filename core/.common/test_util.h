#ifdef FLAG_TESTMODE
#define Test(x) Test_##x
#else
#define Test(x) x
#endif