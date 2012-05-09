#ifndef HEADER_INCLUDE
#define HEADER_INCLUDE
/*
 * 包含一些公共的头文件
 * */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <string.h>

/*
 * 定义一些常用的数据类型的宏
 * */

#define UINT8 unsigned char
#define UINT32 unsigned int 

#define print_func(str) \
    printf("function:%s\n",(str))
//定义图片的结构
struct picture_T
{
    UINT8* data;
    int bpp;
    int width;
    int height;
};

typedef struct picture_T picture_T;

typedef struct histogram
{
    int data[256];
    int sum;
    int max;
    int max_v;
    int min;
    int min_v;
} histogram;

#endif
