#include "pretreat.h"

/*灰度化
 * 返回的指针是因为要把原来的指向数据的指针返回,不然就可能引起内存泄漏
 * */

UINT8*	graying(picture_T *pic)
{
    int	    width	= 0;
    int	    height	= 0;
    int	    i		= 0;
    int	    j		= 0;
    UINT8*  tmp		= NULL;
    UINT8*  tmp1	= NULL;

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);


    assert(pic);
    assert(pic->data);
    assert(pic->width > 0 && pic->height > 0);

    width   = pic->width;
    height  = pic->height;
    tmp	    = (UINT8*)malloc(height * width * sizeof(UINT8));

    assert(tmp);
    //printf("func:%s,pic->bpp = %d\n",__func__,pic->bpp);
    
    if(pic->bpp == 32)
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

	tmp1 = pic->data;
	pic->data = tmp;

	return tmp1;
    }
    else if(pic->bpp == 1)
    {
	printf("func:%s,this pic is 二值图\n",__func__);
	assert(0);
	return NULL;
    }
    else if(pic->bpp == 8)
    {
	free(tmp);
	tmp = NULL;
	return NULL;
    }
}

/*直方图的统计
 * pic:要统计的图片
 * hist:直方图
 * */
void histogram_func(picture_T *pic,histogram *hist)
{
    int width = 0;
    int height = 0;
    int i = 0;
    int j = 0;
    int min = 0;
    int max = 0;

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);

    assert(pic);
    assert(pic->data);
    assert(pic->width > 0 && pic->height > 0);
    assert(hist);
    
    width = pic->width/3;
    height = pic->height/3;

    min = 100000;
    max = 0;
    for(i = 0; i < 256;i++)
    {
	hist->data[i] = 0;
    }
    hist->sum = 0;

    for(i = height - 1; i < (2*height - 1); i++)
    {
	for(j = width - 1; j < (2*width - 1); j++)
	{
	    hist->data[pic->data[i*(pic->width)+j]]++;
	}
    }

    for(i = 0; i < 256; i++)
    {
	if(hist->data[i] > max)
	{
	    max = hist->data[i];
	    hist->max = i;
	}

	if(hist->data[i] < min)
	{
	    hist->min = i;
	    min = hist->data[i];
	}
    }
    hist->sum = height * width;
}

/*
 * 二值化
 * pic:要进行二值化的图片
 * threshold:阈值
 * */

void binarization(picture_T*pic,int threshold)
{

    int i	= 0;
    int j	= 0;
    int width	= 0;
    int height	= 0;

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);


    assert(pic);
    assert(pic->data);
    assert(pic->width > 0 &&  pic->height > 0);

    width	= pic->width;
    height	= pic->height;
 
    
    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    if(pic->data[i*width+j] >= threshold) 
	    {
		pic->data[i*width+j] = 255;
	    }
	    else
	    {
		pic->data[i*width+j] = 0;
	    }
	}
    }

}

/*
 * 冒泡排序
 * ch:要进行排序的数组
 * num:数组的大小
 * 返回的是这个数组的中间的一个值
 * */
static UINT8 get_median_value(UINT8* ch,int num)
{
    int i   = 0;
    int j   = 0;

    assert(ch);
    assert(num > 0);
    
    for(i = 0; i < (num/2 + 1); i++)
    {
	for(j = (num - 1); j > i; j--)
	{
	    if(ch[j - 1] > ch[j] )
	    {
		UINT8 a = ch[j - 1];
		ch[j-1] = ch[j];
		ch[j] = a;
	    }
	}
    }

    return ch[num/2];

}

//中值滤波
void filter(picture_T* pic) 
{
    int	    width   = 0;
    int	    height  = 0;
    UINT8*  data    = NULL;
    int	    i	    = 0;
    int	    j	    = 0;
    int	    k	    = 0;
    UINT8   mask[5] = {0};

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);


    assert(pic);
    assert(pic->data);
    assert(pic->width > 0 && pic->height > 0);

    width   = pic->width;
    height  = pic->height;
    data    = pic->data;

    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    for(k = -2; k <= 2; k++)
	    {
		int index = i + k;

		if(index < 0)
		{
		    index = index + height;
		}
		else if(index >= height)
		{
		    index = index - height;
		}
		
		mask[k+2] = data[index * width + j];
	    }
	    
	    data[i * width + j] = get_median_value(mask,5);
	}
    }
}

/*大津阈值法
 *根据直方图的统计然后通过大津阈值法动态的求出阈值
 原理:当背景的像素与目标的像素方差最大时,那么这个时候的阈值就是最佳分割背景和目标的阈值
 */
int otsu(histogram* hist)
{
    int U1	    = 0;
    long long U2    = 0;
    int i	    = 0;
    int j	    = 0;
    int sum	    = 0;
    int* data	    = NULL; 
    long long nsum  = 0;
    int Threshold   = 0;
    double dMax	    = 0;
    double dVax	    = 0;
    double u	    = 0;
    double v	    = 0;

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);

    assert(hist);
    assert(hist->data);
 
    sum = hist->sum;
    data = hist->data;
    
    for(i = 0; i < 256;i++)
    {
	//printf("hist[%d]:%d\n",i,data[i]);
	nsum += i * data[i];
    }
    //printf("nsum:%lld\n",nsum);
    
    for(j = 0;j < 256; j++)
    {
	u = 0;
	v = 0;
	U1 = 0;
	U2 = 0;

	for(i = 0; i < j; i++)
	{
	    U1 += data[i];
	    U2 += (data[i]*i);
	}
//	printf("U1:%d,U2:%lld\n",U1,U2);

	if(U1 > 0)
	{
	    u = U2/(U1*1.0);
	}
	else
	{
	    u = 0;
	}

	if((sum - U1) > 0)
	{
	    v = (nsum - U2)/((sum-U1)*1.0);
	}
	else
	{
	    v = 0;
	}
    
//	printf("u:%f,v:%f\n",u,v);
	dVax = sqrt(U1) * sqrt(sum - U1) * (u - v) * (u - v);
//	printf("j:%d,dvax:%f,sum:%d\n",j,dVax,sum);

	if(dVax > dMax)
	{
	    dMax = dVax;
	    Threshold = j;
	}
    }
    
//    printf("Threshold:%d\n",Threshold);
    return Threshold;
}

//另外一种取得阈值的方法
int get_histogram_value1(histogram* hist)
{
    int max = 0;
    int min = 0;

    assert(hist);
    assert(hist->max >= 0 && hist->min >= 0);
    max = hist->max;
    min = hist->min;

    return int(hist->max - ((hist->max - hist->min) / 3));
}

/*
 * 这也是另外的取得阈值的方法
 * T0 = 128
 * T(i+1) = (av1(T(i)) + (av2(T(i))))/2
 *
 * 当T(i+1) = T(i) 就是最好的阈值
 * */
int get_histogram_value2(histogram*hist)
{
    int i	    = 0;
    int T0	    = 128;
    int T1	    = 128;

    assert(hist);

    do 
    {
	double av1 = 0;
	double av2 = 0;
	int    n   = 0;
	double sum = 0;
	for(i = 0; i < T0; i++)
	{
	    n += hist->data[i];
	    sum += (hist->data[i] * i);
	}

	av1 = sum / n;

	n = 0;
	sum = 0;

	for(i = T0; i < 256;i++)
	{
	    n += hist->data[i];
	    sum += (hist->data[i] * i);
	}

	av2 = sum / n;

	T0 = T1;
	T1 = (av1 + av2)/2;
    }while(T0 != T1);

    return T0;
}

/*
 * 1.计算图像的直方图
 * 2.进行直方图均衡化
 *
 * 这个函数没有使用到
 */
void Hist_equalization(picture_T*pic)
{
    int	i	    = 0;
    int j	    = 0;
    int width	    = 0;
    int height	    = 0;
    UINT8*data	    = NULL;
    double* hist    = NULL;
    double* p	    = NULL;

    assert(pic && pic->data);
    assert(pic->width > 0 && pic->height >0);

    width	    = pic->width;
    height	    = pic->height;
    data	    = pic->data;
    
    hist = (double*)calloc(256,sizeof(double));
    p	 = (double*)calloc(256,sizeof(double));

    assert(hist);

    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    hist[data[i*width+j]]++;
	}
    }

    for(i = 0; i < 256; i++)
    {
	hist[i] = hist[i] / (width * height * 1.0);
    }

    for(i = 0; i < 256; i++)
    {
	for(j = 0; j <= i; j++)
	{
	    p[i] += hist[j];
	}
    }
    
    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    int pixel = (p[data[i*width+j]]*255+0.5);
	    data[i*width+j] = pixel;
	}
    }

    free(p);
    free(hist);
}
