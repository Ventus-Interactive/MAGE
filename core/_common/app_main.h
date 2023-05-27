#ifndef App_Main
#define App_Main(stmts...) int main(void) { stmts }
#define Test_App_Main(name, stmts...) int main(void) { printf(" :Testing App(%s): \n", name); printf("### Begin\n\n"); stmts printf("\n### End\n"); }
#endif