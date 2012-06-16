#ifndef CHARBAR_INCLUDE
#define CHARBAR_INCLUDE

#ifdef __cplusplus
extern "C"
{
    #include "header.h"
    extern char* Iden_charbar(picture_T* pic);
    extern char* sobel(picture_T*pic);
}
#else
    #include "header.h"
    extern char* Iden_charbar(picture_T* pic);
    extern char* sobel(picture_T*pic);
#endif

#endif
