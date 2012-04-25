#ifndef CHARBAR_INCLUDE
#define CHARBAR_INCLUDE

#ifdef __cplusplus
extern "C"
{
    #include "header.h"
    UINT8* Identify_charbar(const UINT8* tmp,int width,int height);
}
#else
    #include "header.h"
    UINT8* Identify_charbar(const UINT8* tmp,int width,int height);
#endif

#endif
