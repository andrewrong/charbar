#ifndef PRETREAT_INCLUDE
#define PRETREAT_INCLUDE
#include "header.h"

/*
 * 接口介绍
 * graying:灰度化
 * binarization:二值化
 * histogram_func:直方图统计
 * filter:中值滤波
 * ostu:大津法求阈值
 * get_histogram_value1:一种求阈值的方法
 * get_histogram_value2:另一种求阈值的方法
 * */

extern UINT8*	graying(picture_T* pic);
extern void binarization(picture_T* pic,int threshold);
extern void histogram_func(picture_T* pic,histogram* hist);
extern void filter(picture_T* pic);
extern int  otsu(histogram*hist);
extern int  get_histogram_value1(histogram*hist);
extern int  get_histogram_value2(histogram*hist);

extern void Hist_equalization(picture_T* pic);
#endif
