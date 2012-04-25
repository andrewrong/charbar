#ifndef HEADER_INCLUDE
#define HEADER_INCLUDE

#define UINT8 unsigned char
#define UINT32 unsigned int 

//定义图片的结构
struct picture_T
{
    UINT8* data;
    int bpp;
    int width;
    int height;
};

typedef struct picture_T picture_T;

#endif
