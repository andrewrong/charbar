#include "pretreat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

UINT8* graying(picture_T *pic)
{
    int width = 0;
    int height = 0;
    int i = 0;
    int j = 0;
    UINT8 *tmp = NULL;
    assert(pic);
    assert(pic->data);

    width = pic->width;
    height = pic->height;
    tmp = (UINT8*)malloc(height * width * sizeof(UINT8));

    assert(tmp);
    printf("pic->bpp = %d\n",pic->bpp);
    
    if(pic->bpp == 1)
    {
	memcpy(tmp,pic->data,height*width*sizeof(UINT8));
    }
    else
    {
	for(i = 0; i < height; i++)
    	{
    	    for(j = 0; j < width; j++)
    	    {
    	        UINT32 r = pic->data[i * width * 4 + j * 4];
    	        UINT32 g = pic->data[i * width * 4 + j * 4 + 1];
    	        UINT32 b = pic->data[i * width * 4 + j * 4 + 2];
    	        //printf("rgb = (%u,%u,%u) ",r,g,b);
    	        tmp[i*width + j] = (UINT8)((r * 77 + g * 151 + b * 28) >> 8);
		//printf("%d ",tmp[i*width+j]);
    	    }
    	    //printf("\n");
    	}
    }
    return tmp; 
}

void binarization(UINT8*tmp,int width,int height)
{

    int i = 0;
    int j = 0;

    assert(tmp);
    assert(width > 0 &&  height > 0);
 
    
    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    if(tmp[i*width+j] >= 128) 
	    {
		tmp[i*width+j] = 255;
	    }
	    else
	    {
		tmp[i*width+j] = 0;
	    }
	}
    }

}
