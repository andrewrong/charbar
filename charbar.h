#ifndef CHARBAR_INCLUDE
#define CHARBAR_INCLUDE

#ifdef __cplusplus
extern "C"
{
    #include "header.h"
    #define  EAN    60
    #define  EAN_M  95
    char* Iden_charbar(picture_T* pic);
}
#else
    #include "header.h"
    #define  EAN    60
    #define  EAN_M  95
    char* Iden_charbar(picture_T* pic);
#endif

#endif
