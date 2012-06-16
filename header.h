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

/*定义图片的结构
 * data:指向图像原始的数据的指针
 * bpp:每一个像素的bits
 * width:图像的宽度
 * height:图像的高度
 * */
struct picture_T
{
    UINT8* data;
    int bpp;
    int width;
    int height;
};

typedef struct picture_T picture_T;

/*
 * 直方图的数据结构
 * data:直方图的数据
 * sum:元素的个数
 * max:直方图中最大的灰度级
 * min:直方图中最小的灰度级

 * */
typedef struct histogram
{
    int data[256];
    int sum;
    int max;
    int min;
    int av;
} histogram;

#endif
