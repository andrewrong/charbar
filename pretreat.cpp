#include "pretreat.h"

#define SIZE1  3		
#define SIZE2  7		

#define STANDW 480 
#define STANDH 320 

#define BLOCK  16 
/*灰度化
 * 返回的指针是因为要把原来的指向数据的指针返回,不然就可能引起内存泄漏
 * */

void	graying(picture_T *pic)
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

	free(pic->data);
	pic->data = tmp;
    }
    else if(pic->bpp == 1)
    {
	printf("func:%s,this pic is 二值图\n",__func__);
	assert(0);
    }
    else if(pic->bpp == 8)
    {
	free(tmp);
	tmp = NULL;
	assert(0);
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
    long long s = 0;
    int max = 0;
    int min = 100000;

/*
 * 显示进入的函数
 * */
    printf("function:%s\n",__func__);

    assert(pic);
    assert(pic->data);
    assert(pic->width > 0 && pic->height > 0);
    assert(hist);

    memset(hist,0,sizeof(histogram));
#if 0    
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
#endif

    width = pic->width;
    height = pic->height;

    memset(hist->data,0,256); 

    hist->sum = width*height;
    hist->av = 0;
    for(i = 0; i < (height); i++)
    {
	for(j = 0; j < width; j++)
	{
	    hist->data[pic->data[i*(pic->width)+j]]++;
	    s += pic->data[i*pic->width+j];
	}
    }

    hist->av = (s) /(hist->sum);
    printf("average grey value:%d\n",hist->av);

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
static void bubble_sort(UINT8* ch,int num)
{
    int i   = 0;
    int j   = 0;

    assert(ch);
    assert(num > 0);


    for(i = 0; i < (num - 1); i++)
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
}

static UINT8 get_median_value(UINT8* ch,int num)
{
    bubble_sort(ch,num);
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
    int i	= 0;
    int j	= 0;
    int num	= 0;
    double w0	= 0;
    double w1	= 0;
    double u0	= 0;
    double u1	= 0;
    long long sum1 = 0;
    long long sum2 = 0;
    long long num1 = 0;
    long long num2 = 0;
    double g[256];
    double max     = 0;

    assert(hist);
    memset(g,0,256);

    num	= hist->sum;

    for(i = 0; i < 256; i++)
    {
	for(j = 0; j <= i; j++)	
	{
	    num1 += hist->data[j];
	    sum1 += (hist->data[j] * j);
	}
	
	w0 = (num1*1.0)/num;
	if(num1 == 0)
	{
	    u0 = 0;
	}
	else
	{
	    u0 = (sum1*1.0)/num1;
	}
	
	num1 = 0;
	sum1 = 0;

	for(j = i+1;j < 256; j++)
	{
	    num2 += hist->data[j];
	    sum2 += (hist->data[j] * j);
	}

	w1 = (num2*1.0)/num;
	if(num2 == 0)
	{
	    u1 = 0; 
	}
	else
	{
	    u1 = (sum2*1.0)/num2;
	}

	num2 =  0;
	sum2 =  0;

	g[i] = w0*w1*(u1-u0)*(u1-u0); 
    }
    
    max = 0;
    j	= 0;
    for(i = 0; i < 256; i++)
    {
	if(g[i] > max)
	{
	    max = g[i];
	    j = i;
	}
    }
    return j;
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


static inline UINT8 pixel(UINT8*data,int x,int y,int width,int height)
{
    if(x >= width)
    {
	x = x - 1 ;
    }

    if(y >= height)
    {
	y = y - 1;
    }

    return data[y*width+x];
}

static UINT8 getpixel(UINT8*data,double ux,double uy,int width,int height)
{
    int	    x = int(ux);
    int	    y = int(uy);
    double  u = 0;
    double  v = 0;
    double  re = 0;
    UINT8   a1 = 0;
    UINT8   a2 = 0;
    UINT8   a3 = 0;
    UINT8   a4 = 0;

    if(x > ux)
    {
	--x;
    }
    
    if(y > uy)
    {
	--y;
    }

    u = ux - x;
    v = uy - y;
    
    a1 = pixel(data,x,y,width,height);
    a2 = pixel(data,x+1,y,width,height);
    a3 = pixel(data,x,y+1,width,height);
    a4 = pixel(data,x+1,y+1,width,height);
    
    re = (1-u)*(1-v)*a1+(v*(1-u))*a3+(u*(1-v))*a2+(u*v)*a4;
    return (UINT8)((int)re);
}

void bilinear(picture_T* pic)
{
    int		i = 0;
    int		j = 0;
    int	width	  = 0;
    int height	  = 0;
    int uwidth	  = STANDW;
    int uheight	  = STANDH;
    
    UINT8*data	  = NULL;
    UINT8*udata	  = NULL;

    assert(pic && pic->data);
    assert(pic->width > 0 && pic->height > 0);

    data    = pic->data;
    width   = pic->width;
    height  = pic->height;
    uwidth  = STANDW;
    uheight = STANDH;
    udata   = (UINT8*)calloc(uwidth*uheight,sizeof(UINT8));


    assert(udata);

    for(i = 0; i < uheight; i++)
    {
	for(j = 0; j < uwidth; j++)
	{
	    double ux = fabs((width*(j+0.49999)/(uwidth*1.0)) - 0.5);
	    double uy = fabs((height*(i+0.49999)/(uheight*1.0)) - 0.5);
	    //printf("(%d,%d)\n",i,j);
	    //printf("(%lf,%lf)\n",ux,uy);
	    udata[i*uwidth+j] = getpixel(data,ux,uy,width,height);
	}
    }
    
    printf("hello,world!\n");
    pic->height = uheight;
    pic->width = uwidth;
    pic->data = udata;

    free(data);
}

static void getud(UINT8*data,int width,int height,int*u,int*d)
{
    int i	    = 0;
    int j	    = 0;
    long long sum   = 0;
    double sum1	    = 0;
    int	udata[256]  = {0};

    assert(data);
    assert(width > 0 && height > 0);
    
    memset(udata,0,256);

    for(i = 0 ; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    udata[data[i*width+j]]++;
	    sum += data[i*width+j];
	}
    }

    *u = sum/(width*height);

    for(i = 0; i < 256; i++)
    {
	sum1 += ((i - *u) * (i - *u) * (udata[i]*1.0)/(width*height));
    }

    *d = (int)sum1;
}

void delshine2(picture_T* pic,histogram* hist)
{
    int	width	    = 0;
    int height	    = 0;
    int	u	    = 0;
    int i	    = 0;
    int j	    = 0;
    int k	    = 0;
    int h	    = 0;
    long long sum	    = 0;
    
    UINT8*sdata		    = NULL;
    UINT8*gdata1	    = NULL;
    UINT8*gdata2	    = NULL;
    picture_T*	tmp = NULL;     

    int gwidth	    = 0;
    int gheight	    = 0;
    int bwidth	    = 0;
    int bheight	    = 0;

    int    ui	    = 0;
    int    di	    = 0;

    assert(pic && pic->data && hist);
    assert(pic->width > 0 && pic->height > 0);

    width	= pic->width;
    height	= pic->height;
    bwidth	= BLOCK;
    bheight	= BLOCK;
    gwidth	= STANDW/BLOCK;
    gheight	= STANDH/BLOCK;
    sdata	= pic->data;

    gdata1	= (UINT8*)calloc(bwidth*bheight,sizeof(UINT8));
    gdata2	= (UINT8*)calloc(gwidth*gheight,sizeof(UINT8));
    tmp		= (picture_T*)calloc(1,sizeof(picture_T));

    assert(gdata1 &&  gdata2 && tmp);
    

    u = hist->av;
    
    for(i = 0; i < gheight; i++)
    {
	for(j = 0; j < gwidth; j++)
	{
	    for(k = 0; k < bheight; k++)
	    {
		for(h = 0; h < bwidth; h++)
		{
		    gdata1[k*bwidth+h] = sdata[(i*bheight+k)*width+(j*bwidth+h)];
		}
	    }
	    getud(gdata1,bwidth,bheight,&ui,&di);
	    //printf("ui=%d,di=%d\n",ui,di);
	    sum = ui - u + (0.3)*(sqrt(di));
	     if( sum < 0)
	     {
		gdata2[i*gwidth+j] = 0;
	     }
	     else if( sum > 255) 
	     {
		gdata2[i*gwidth+j] = 255;
	     }
	     else
	     {
		gdata2[i*gwidth+j] = sum;
	     }
	    ui = 0;
	    di = 0;
	}
    }
    tmp->data = gdata2;
    tmp->width = gwidth;
    tmp->height = gheight;
    tmp->bpp = 8;
    
    bilinear(tmp);

    free(gdata2);
    gdata2 = tmp->data;

    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    sdata[i*width+j] = abs(sdata[i*width+j] - gdata2[i*width+j]);
	}
    }

   // pic->data = gdata2;
    free(gdata1);
    free(gdata2);
    free(tmp);
}
