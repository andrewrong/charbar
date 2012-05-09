#include "charbar.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>

#define MATRIX_W 60
#define AERA_TH     40

typedef struct elem
{
    char ch;
    int is_odd;
} elem;

//索引归一值
static elem htab[60];
//奇偶对应的值
static int ch_data[32] = {'0'};

void display(int *arr,int num)
{
    int i = 0;
    for(i = 0; i < num; i++)
    {
	printf("data[%d]:%d\n",i,arr[i]);
    }


    printf("********************************\n");
}

void init()
{
    print_func(__func__);

    htab[53].ch = '0';
    htab[55].ch = '3';
    htab[24].ch = '4';
    htab[35].ch = '5';
    htab[22].ch = '6';
    htab[42].ch = '9';
    
    htab[53].is_odd = 1;
    htab[55].is_odd = 1;
    htab[24].is_odd = 1;
    htab[35].is_odd = 1;
    htab[22].is_odd = 1;
    htab[42].is_odd = 1;
    
    htab[23].ch = '0';
    htab[25].ch = '3';
    htab[54].ch = '4';
    htab[45].ch = '5';
    htab[52].ch = '6';
    htab[32].ch = '9';
    
    
    htab[23].is_odd = 0;
    htab[25].is_odd = 0;
    htab[54].is_odd = 0;
    htab[45].is_odd = 0;
    htab[52].is_odd = 0;
    htab[32].is_odd = 0;
    
    ch_data[0] = '0';
    ch_data[1] = 'b';
    ch_data[2] = 'b';
    ch_data[3] = 'b';
    ch_data[4] = 'b';
    ch_data[5] = 'b';
    ch_data[6] = 'b';
    ch_data[7] = 'b';
    ch_data[8] = 'b';
    ch_data[9] = 'b';
    ch_data[10] = 'b';
    ch_data[11] = '1';
    ch_data[12] = 'b';
    ch_data[13] = '2';
    ch_data[14] = '3';
    ch_data[15] = 'b';
    ch_data[16] = 'b';
    ch_data[17] = 'b';
    ch_data[18] = 'b';
    ch_data[19] = '4';
    ch_data[20] = 'b';
    ch_data[21] = '7';
    ch_data[22] = '8';
    ch_data[23] = 'b';
    ch_data[24] = 'b';
    ch_data[25] = '5';
    ch_data[26] = '9';
    ch_data[27] = 'b';
    ch_data[28] = '6';
    ch_data[29] = 'b';
    ch_data[30] = 'b';
    ch_data[31] = 'b';
}

/*
 * 统计每一行的黑色点的个数,赋值给hor这个数组
 * */
static void StatisticH(UINT8*data,int width ,int height,int* arPixelH)
{
    int i = 0;
    int j = 0;

    print_func(__func__);

    assert(data && arPixelH);
    assert(width > 0 && height > 0);

    for(i = 0; i < height; i++)
    {
	for(j = 0;j < width; j++)
	{
	    if(data[i*width+j] == 0)
	    {
		arPixelH[i]++;
	    }
	}
    }
}

static void StatisticV(UINT8*data,int width,int* arPixelV,int topImage,int bottomImage)
{
    int i = 0;
    int j = 0;

    assert(data && arPixelV);
    assert(width > 0 && topImage >= 0 && bottomImage >= 0);
    
    print_func(__func__);

    for(j = 0; j < width; j++)
    {
	for(i = topImage; i <= bottomImage; i++)
    	{
	    if(data[i*width+j] == 0) 
	    {
		arPixelV[j]++;
	    }
    	}
    }
}

/*
 * arPixelH:统计了每一行的黑色像素的个数
 * height:高度
 * armarkH:用来标记哪些行是可以用来扫描的
 * topImage:上边缘
 * bottomImage:下边缘
 * */

int BorderH(int* arPixelH,int height,int* armarkH,int*	topImage,int* bottomImage)
{
    int i	= 0;
    int j	= 0;
    double sum	= 0;
    int	 count	= 0;
    double aver	= 0;

    assert(arPixelH && armarkH && topImage && bottomImage);
    assert(height > 0);

    print_func(__func__);

    for(i = 0; i < height; i++)
    {
	if(arPixelH[i] > 0)
	{
	    count++;
	    sum += arPixelH[i];
	}
    }

    aver = (sum) / count;
    

    //计算符合条件的行
    for(i = 0; i < (height - 1); i++)
    {
	int dis = abs(arPixelH[i] - arPixelH[i+1]);

	if(dis <= 5 && arPixelH[i] >= (0.75)*aver)
	{
	    armarkH[i] = 1;
	}
    }

    //用来确定上下边界
    for(i = 0; i < (height - 40); i++)
    {
	int iCount = 0;
	for(j = 0; j < 40; j++)
	{
	    if(armarkH[i+j] == 1)
	    {
		iCount++;
	    }
	}

	if(iCount >= 37)
	{
	    *topImage = i; 
	    *bottomImage = i + 40;
	    break;
	}
    }

    if(topImage == bottomImage)
    {
	return -1;
    }

    return 0;
}

/*
 *BorderV
 *用来确定左右边界
 * */

void BorderV(int* arPixelV,int width,int* armarkV)
{
    int i	= 0;
    int tmpmax	= 0;

    assert(arPixelV && armarkV);
    assert(width > 0);

    print_func(__func__);

    for(i = 0; i < width; i++)
    {
	if(arPixelV[i] > tmpmax)
	{
	    tmpmax = arPixelV[i];
	}
    }

    for(i = 0; i < width; i++)
    {
	if(arPixelV[i] >= (0.75 * tmpmax))	
	{
	    armarkV[i] = 1;
	}
    }
}

/*
 * distance:存放距离
 * armark:已经被定好的条空
 * tmp:本身是用来存放每一个边界的坐标
 * */
static int get_distance1(int width,int* distance,int* armarkV,double*average)
{
    int i	= 0;
    int j	= 0;
    int	k	= 0;
    int left	= 0;
    int*tmp	= (int*)calloc(100,sizeof(int));

    assert(width > 0);
    assert(distance && armarkV);

    print_func(__func__);

    for(i = 0; i < (width - 1); i++)
    {
	if((armarkV[i] ^ armarkV[i + 1]) == 1)
	{
	    tmp[j++] = i;
	}
    }
    
    for(i = 0; i < j - 1; i++)
    {
	distance[i] = tmp[i+1] - tmp[i];
    }
    
    if(MATRIX_W == j)
    {
	printf("条形码的图像很标准\n");
	left = 0;
    }
    else if(j < MATRIX_W)
    {
	printf("条形码的图像不是很好,无法检测\n");
	printf("j:%d\n",j);
	return -1;
    }
    else
    {
	for(i = 0; i < (j - 1); i++)
	{
	    float a = distance[i+1] / (distance[i] * 1.0); 

	    if(a >= 0.5 && a <= 2)
	    {
		for(k = i; k < i + 59; k++)
		{
		    distance[k-i] = distance[k];
		}
		left = i;
		break;
	    }
	}
    }

    *average = (tmp[left+59] - tmp[left])/(95*1.0);
    left = tmp[left];

    free(tmp);
    tmp = NULL;
    return left;
    //进行判断,确定左边缘
}

static int sort(int* tmp,int n)
{
    int i	= 0;
    int j	= 0;

    assert(tmp);

    for(i = 0; i < (n - 1); i++)
    {
	for(j = 0; j < (n - i - 1); j++)
	{
	    if(tmp[j] > tmp[j+1])
	    {
		int elem = tmp[j];
		tmp[j] = tmp[j+1];
		tmp[j+1] = elem;
	    }
	}
    }

    return tmp[n/2];
}

/*
 * 先上下边缘都确定时,然后计算每一行的边界变化的坐标,并且保存在matrix
 * */
static int* get_distance2(int width,int topImage,int bottomImage,int left,int**matrix,UINT8*data)
{
    int	    i	    = 0;
    int	    j	    = 0;
    int	    k	    = 0;
    int*    distance = (int*)calloc(59,sizeof(int));
    int*    tmp	    = (int*)calloc(bottomImage - topImage,sizeof(int));

    assert(distance && tmp);
    
    for(i = topImage; i <= bottomImage; i++)
    {
	k = 0;
	for(j = (left - 5); j < width && k < 60; j++)
	{
	    if((data[i*width+j-1]^data[i*width+j]) == 255)
	    {
		matrix[i-topImage][k++] = j;
	    }
	}
    }
    
    //因为matrix在一开始都被赋值为0,从上面可知没有一个边界会是从0开始的，所以可以作为一种终止标准

    for(i = 0; i <= (bottomImage - topImage); i++)
    {
	for(j = 0; j < 59; j++)
	{
	    matrix[i][j] = matrix[i][j+1] - matrix[i][j];
	}
    }
    //matrix存放的是每一个条空的距离

    for(i = 0; i < 59;i++)
    {
	for(j = 0; j <= (bottomImage - topImage); j++)
	{
	    tmp[j] = matrix[j][i];
	}

	distance[i] = sort(tmp,bottomImage-topImage+1);
    }
    
    display(distance,59);
    free(tmp);
    tmp = NULL;
    
    return distance;
}


static int get_char(int c1,int c2,int c3,int c4,UINT8*str,int k)
{
    print_func(__func__);

    //A集合
    if(c1 == 3 && c2 == 2 && c3 == 1 && c4 == 1)
    {
	str[k] = '0';
	return 0;
    }

    if(c1 == 2 && c2 == 2 && c3 == 2 && c4 == 1)
    {
	str[k] = '1';
	return 0;
    }

    if(c1 == 2 && c2 == 1 && c3 == 2 && c4 == 2)
    {
	str[k] = '2';
	return 0;
    }

    if(c1 == 1 && c2 == 4 && c3 == 1 && c4 == 1)
    {
	str[k] = '3';
	return 0;
    }

    if(c1 == 1 && c2 == 1 && c3 == 3 && c4 == 2)
    {
	str[k] = '4';
	return 0;
    }

    if(c1 == 1 && c2 == 2 && c3 == 3 && c4 == 1)
    {
	str[k] = '5';
	return 0;
    }

    if(c1 == 1 && c2 == 1 && c3 == 1 && c4 == 4)
    {
	str[k] = '6';
	return 0;
    }

    if(c1 == 1 && c2 == 3 && c3 == 1 && c4 == 2)
    {
	str[k] = '7';
	return 0;
    }

    if(c1 == 1 && c2 == 2 && c3 == 1 && c4 == 3)
    {
	str[k] = '8';
	return 0;
    }

    if(c1 == 3 && c2 == 1 && c3 == 1 && c4 == 2)
    {
	str[k] = '9';
	return 0;
    }

//B集合
    if(c1 == 1 && c2 == 1 && c3 == 2 && c4 == 3)
    {
	str[k] = '0';
	return 1;
    }

    if(c1 == 1 && c2 == 2 && c3 == 2 && c4 == 2)
    {
	str[k] = '1';
	return 1;
    }

    if(c1 == 2 && c2 == 2 && c3 == 1 && c4 == 2)
    {
	str[k] = '2';
	return 1;
    }

    if(c1 == 1 && c2 == 1 && c3 == 4 && c4 == 1)
    {
	str[k] = '3';
	return 1;
    }

    if(c1 == 2 && c2 == 3 && c3 == 1 && c4 == 1)
    {
	str[k] = '4';
	return 1;
    }

    if(c1 == 1 && c2 == 3 && c3 == 2 && c4 == 1)
    {
	str[k] = '5';
	return 1;
    }

    if(c1 == 4 && c2 == 1 && c3 == 1 && c4 == 1)
    {
	str[k] = '6';
	return 1;
    }

    if(c1 == 2 && c2 == 1 && c3 == 3 && c4 == 1)
    {
	str[k] = '7';
	return 1;
    }

    if(c1 == 3 && c2 == 1 && c3 == 2 && c4 == 1)
    {
	str[k] = '8';
	return 1;
    }

    if(c1 == 2 && c2 == 1 && c3 == 1 && c4 == 3)
    {
	str[k] = '9';
	return 1;
    }
    
    str[k] = 'b';
    return -1;
}

static int change_distance(int c,double average)
{
    double a = c / average;

    print_func(__func__);

    if(a >= 0.5 && a < 1.5)
    {
	return 1;
    }
    else if(a >= 1.5 && a < 2.5)
    {
	return 2;
    }
    else if(a >= 2.5 && a < 3.5)
    {
	return 3;
    }
    else if(a >= 3.5 && a < 4.5)
    {
	return 4;
    }

    return 1;
}

static int is_valid(UINT8*result)
{
    int i   = 0;
    int sum1 = 0;
    int sum2 = 0;

    assert(result);

    for(i = 1; i < 12; i += 2)
    {
	sum1 += result[i];
    }
    for(i = 0; i < 12; i++)
    {
	sum2 +=	result[i];
    }

    sum1 = sum1*3 + sum2;

    if((10 - sum1 % 10) == (result[12] - '0'))
    {
	return 1;
    }
    else
    {
	return 0;
    }
}

/*第二种检测方式*/
static int get_key(int T1,int T2,int T)
{
    double a = T1 / (T*1.0);
    double b = T2 / (T*1.0);
    int k1 = 0;
    int k2 = 0;

    double c1 = 2.5/7;
    double c2 = 3.5/7;
    double c3 = 4.5/7;
    
    if(a < c1)
    {
	k1 = 2;
    }
    else if(a >= c1 && a < c2)
    {
	k1 = 3;
    }
    else if(a >= c2 && a < c3)
    {
	k1 = 4;
    }
    else
    {
	k1 = 5;
    }
    
    if(b < c1)
    {
	k2 = 2;
    }
    else if(b >= c1 && b < c2)
    {
	k2 = 3;
    }
    else if(b >= c2 && b < c3)
    {
	k2 = 4;
    }
    else
    {
	k2 = 5;
    }

    return k1*10 + k2;
}


static int my_switch(int re,int c1,int c2,int c3,int c4,UINT8*result,int k)
{
    switch(re)
    {
	case 44:
	    {
		if(c3 > c4)
		{
		    result[k] = '1';
			return 0;
		}
		else
		{
		    result[k] = '7';
		    return 0;
		}
		break;
	    }
	case 33:
	    {
		if(c2 > c3)
		{
		    result[k] = '8';
		    return 0;
		}
		else
		{
		    result[k] = '2';
		    return 0;
		}
		break;
	    }
	case 34:
	    {
		if(c1 > c2)
		{
		    result[k] = '7';
		    return 1;
		}
		else
		{
		    result[k] = '1';
		    return 1;
		}
		break;
	    }
	case 43:
	    {
		if(c2 > c3)
		{
		    result[k] = '2';
		    return 1;
		}
		else
		{
		    result[k] = '8';
		    return 1;
		}
		break;
	    }
	default:
	    {
		result[k] = htab[re].ch;
		if(htab[re].is_odd)
		{
		    return 0;
		}
		else
		{
		    return 1;
		}
	    }

    }
}


/*
 * 条形码识别函数
 * arPixelH:水平映射的数组
 * arPixelV:垂直映射的数组
 * armarkH:用来记录哪些行是可以用来检测的
 * armarkV：用来保存哪些条是确定的
 * */
char* Iden_charbar(picture_T* pic)
{ 
    int	    width	    = 0;
    int	    height	    = 0;
    int	    topImage	    = 0;
    int	    bottomImage	    = 0;
    int	    left	    = 0;
    double  average	    = 0;
    int	    i		    = 0;
    int	    k		    = 1;
    int	    index	    = 0;

    int*    arPixelH	    = NULL;
    int*    arPixelV	    = NULL;
    int*    armarkH	    = NULL;
    int*    armarkV	    = NULL;
    int*    distance	    = NULL;
    int**   matrix	    = NULL;
    UINT8*  data	    = NULL;
    UINT8*  result1	    = NULL;
    UINT8*  result2	    = NULL;

    assert(pic && pic->data);
    assert(pic->width >= 0 && pic->height >= 0);

    width	=	pic->width;
    height	=	pic->height;
    data	=	pic->data;
    arPixelH	=	(int*)calloc(height,sizeof(int));
    arPixelV	=	(int*)calloc(width,sizeof(int));
    armarkV	=	(int*)calloc(width,sizeof(int));
    armarkH	=	(int*)calloc(height,sizeof(int));
    distance	=	(int*)calloc(100,sizeof(int));
    result1	=	(UINT8*)calloc(14,sizeof(UINT8));
    result2	=	(UINT8*)calloc(14,sizeof(UINT8));

    assert(arPixelH && arPixelV && armarkH && armarkV && distance && result1 && result2);

    /*
     * 1.水平黑色像素统计
     * 2.找到可以检测的上下边缘
     * 3.在上下边缘的基础上进行垂直方向上的像素统计
     * 4.距离的计算
     * 5.进行条形码的识别
     * */
    init();
    //1
    StatisticH(data,width,height,arPixelH);
    
    //2
    if(BorderH(arPixelH,height,armarkH,&topImage,&bottomImage) == -1)
    {
	printf("没有区域是符合要求检测的,so 我没有办法检测\n");
	return "NULL";
    }

    printf("top:%d,bottomImage:%d\n",topImage,bottomImage);

    StatisticV(data,width,arPixelV,topImage,bottomImage);

    //3.2
    BorderV(arPixelV,width,armarkV);

    //4.1
    left = get_distance1(width,distance,armarkV,&average);

    if(left == -1)
    {
	return "NULL";
    }
    printf("left:%d\n",left);
    
    printf("average:%f\n",average);

    matrix = (int**)calloc(bottomImage - topImage + 1, sizeof(int*));

    for(i = 0; i < (bottomImage - topImage + 1); i++)
    {
	matrix[i] = (int*)calloc(60,sizeof(int));
    }
    
    free(distance);
    distance = get_distance2(width,topImage,bottomImage,left,matrix,data);
    

    //5.1 确定前面6个字符
    for(i = 3; i < 27;)
    {
	int c1 = distance[i++];	
	int c2 = distance[i++];	
	int c3 = distance[i++];	
	int c4 = distance[i++];	

	c1 = change_distance(c1,average);
	c2 = change_distance(c2,average);
	c3 = change_distance(c3,average);
	c4 = change_distance(c4,average);
	
	assert(c1 && c2 && c3 && c4);

	if(get_char(c1,c2,c3,c4,result1,k++) == 1)
	{
	    index += pow(2,7-k);
	}
    }
    
    result1[0] = ch_data[index];

    for(i = 32; i < 56;)
    {
	int c1 = distance[i++];	
	int c2 = distance[i++];	
	int c3 = distance[i++];	
	int c4 = distance[i++];	

	c1 = change_distance(c1,average);
	c2 = change_distance(c2,average);
	c3 = change_distance(c3,average);
	c4 = change_distance(c4,average);
	
	get_char(c1,c2,c3,c4,result1,k++);
    }

    result1[13] = '\0';

    k = 1;
    index = 0;

    for(i = 3; i < 27;)
    {
	int c1 = distance[i++];	
	int c2 = distance[i++];	
	int c3 = distance[i++];	
	int c4 = distance[i++];	

	int T1 = c1 + c2;
	int T2 = c2 + c3;
	int T  = c1 + c2 + c3 + c4;

	int re = get_key(T1,T2,T);
	
	if(my_switch(re,c1,c2,c3,c4,result2,k++))
	{
	    index += pow(2,7-k);
	}
    }

    result2[0] = ch_data[index];

    for(i = 32; i < 56;)
    {
	int c1 = distance[i++];	
	int c2 = distance[i++];	
	int c3 = distance[i++];	
	int c4 = distance[i++];	

	int T1 = c1 + c2;
	int T2 = c2 + c3;
	int T  = c1 + c2 + c3 + c4;

	int re = get_key(T1,T2,T);
	
	my_switch(re,c1,c2,c3,c4,result2,k++);
    }
    
    result2[13] = '\0';

    printf("条形码1:%s\n",result1);
    printf("条形码2:%s\n",result2);

    for(i = 0; i < 13; i++)
    {
	if(result1[i] == 'b' && result2[i] != 'b')
	{
	    result1[i] = result2[i];
	}
    }


    for(i = 0; i < (bottomImage - topImage + 1); i++)
    {
	free(matrix[i]);
    }
    free(result2);
    free(matrix);
    free(arPixelH);
    free(arPixelV);
    free(armarkH);
    free(armarkV);
    free(distance);

    return (char*)result1;
}

