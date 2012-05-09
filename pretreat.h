#ifndef PRETREAT_INCLUDE
#define PRETREAT_INCLUDE
#include "header.h"

extern void graying(picture_T* pic);
extern void binarization(picture_T* pic,int threshold);
extern void histogram_func(picture_T* pic,histogram* hist);
extern void filter(picture_T* pic);
extern int  otsu(histogram*hist);
extern int  get_histogram_value1(histogram*hist);
extern int  get_histogram_value2(histogram*hist);
#endif
