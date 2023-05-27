
#ifndef COMMON_SDEFS
#define COMMON_SDEFS


#define COMMON_TAG /*MAGE:API*/ 


// define a type : (i32, signed int)
#define DT(id, from) COMMON_TAG typedef from id;

// define a struct : (Pair, float a, float b)
#define DS(tag, fields) COMMON_TAG typedef struct tag { fields } tag; 

#define LibFn(sig) backend_##sig


#endif
