#include "charbar.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#define MATRIX_W 60

typedef struct elem
{
    char ch;
    int is_odd;
} elem;

//索引归一值
static elem htab[60];
//奇偶对应的值
static int ch_data[32] = {'0'};

void init()
{
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
    ch_data[1] = '0';
    ch_data[2] = '0';
    ch_data[3] = '0';
    ch_data[4] = '0';
    ch_data[5] = '0';
    ch_data[6] = '0';
    ch_data[7] = '0';
    ch_data[8] = '0';
    ch_data[9] = '0';
    ch_data[10] = '0';
    ch_data[11] = '1';
    ch_data[12] = '0';
    ch_data[13] = '2';
    ch_data[14] = '3';
    ch_data[15] = '0';
    ch_data[16] = '0';
    ch_data[17] = '0';
    ch_data[18] = '0';
    ch_data[19] = '4';
    ch_data[20] = '0';
    ch_data[21] = '7';
    ch_data[22] = '8';
    ch_data[23] = '0';
    ch_data[24] = '0';
    ch_data[25] = '5';
    ch_data[26] = '9';
    ch_data[27] = '0';
    ch_data[28] = '6';
    ch_data[29] = '0';
    ch_data[30] = '0';
    ch_data[31] = '0';

}

static int* matrix_init(const UINT8*data,const int width,const int height,int **matrix)
{
    int i = 0, j = 0;
    int k = 0,h = 0;
    long long sum = 0;
    int *line = NULL;

    assert(data);
    assert(matrix);
    assert(width > 0 && height > 0);

    for(i = 0; i < height;i++)
    {
	for(j = 0; j < width; j++)
	{
	    if(j > 0)
	    {
		if(data[i*width+j] ^ data[i*width + j - 1])
		{
		    //printf("[%d,%d] = %d\n",data[i*width+j],data[i*width + j - 1],j);
		    if(h < MATRIX_W)
		    {
			matrix[k][h++] = j;
		    }
		    else
		    {
			h++;
		    }
		}
	    }
	}
	
	//printf("------------------------\n");
	if(h != MATRIX_W)
	{
	    k--;
	}
	k++;
	h = 0;
    }
    printf("有效的行数 = %d\n",k);

    if(k == 0)
    {
	return NULL;
    }

   //k这里表示的是 matrix有效的行数
    for(i = 0; i < k; i++)
    {
	for(j = 0; j < MATRIX_W - 1;j++)
	{
	    matrix[i][j] = matrix[i][j + 1] - matrix[i][j];
//	    printf("(%d,%d) = (%d) ",i,j,matrix[i][j]);
	}
//	printf("\n");
    }

    //计算均值
    line = (int*)malloc(sizeof(int)*(MATRIX_W - 1));
    assert(line);

    for(j = 0; j < (MATRIX_W - 1); j++)
    {
	for(i = 0; i < k; i++)
	{
	    sum += matrix[i][j];
	}
	
	line[j] = sum / k;
	sum = 0;
    }
   
    return line;
}

static int getAT(double a1,double a2)
{
        double a3 = 2.5/7;
	double a4 = 3.5/7;
	double a5 = 4.5/7;
	int AT1 = 0;
	int AT2 = 0;
	int result = 0;

	if(a1 < a3)
	{
	    AT1 = 2;
	}
	else if(a1 < a4)
	{
	    AT1 = 3;
	}
	else if(a1 < a5)
	{
	    AT1 = 4;
	}
	else
	{
	    AT1 = 5;
	}
	
	if(a2 < a3)
	{
	    AT2 = 2;
	}
	else if(a2 < a4)
	{
	    AT2 = 3;
	}
	else if(a2 < a5)
	{
	    AT2 = 4;	
	}
	else
	{
	    AT2 = 5;
	}
	
	return result = AT1*10 + AT2;
}


static void Normalize(const int*line,UINT8*str)
{
    int i = 0;
    int k = 1;
    int odd_even = 0;

    for(i = 3; i < 56;)
    {
	int c1 = line[i++];
	int c2 = line[i++];
	int c3 = line[i++];
	int c4 = line[i++];
	
	double T1 = c1 + c2;
	double T2 = c2 + c3;
	double T = c1 + c2 + c3 + c4;
	int result = 0;

	double a1 = T1/T;
	double a2 = T2/T;

	//printf("c1:%d,c2:%d,c3:%d,c4:%d\n",c1,c2,c3,c4);
	//printf("a1:%f,a2:%f\n",a1,a2);

	
	result = getAT(a1,a2);
	//printf("result:%d\n",result);
	//printf("k = %d\n",k);

	switch(result)
	{
	    case 44:
		{
		    if(c3 < c4)
		    {
			str[k++] = '7';	
		    }
		    else
		    {
			str[k++] = '1';
		    }
		    break;
		}
	    case 33:
		{
		    if(c2 > c3)
		    {
			str[k++] = '8';
		    }
		    else
		    {
			str[k++] = '2';
		    }
		    break;
		}
	    case 34:
		{
		    if(c1 > c2)
		    {
			odd_even += pow(2,6-k);
			str[k++] = '7';
		    }
		    else
		    {
			odd_even += pow(2,6-k);
			str[k++] = '1';
		    }
		    break;
		}
	    case 43:
		{
		    if(c2 > c3)
		    {
			odd_even += pow(2,6-k);
			str[k++] = '2';
		    }
		    else
		    {
			odd_even += pow(2,6-k);
			str[k++] = '8';
		    }
		    break;
		}
	    default:
	    {
		if(!htab[result].is_odd)
		{
		    odd_even += pow(2,6-k);
		    str[k++] = htab[result].ch;
		}
		else
		{
		    str[k++] = htab[result].ch;
		}

		break;
	    }
	}

	if(i == 27)
	{
	    printf("odd_even is %d\n",odd_even);
	    str[0] = ch_data[odd_even];

	    i = 32;
	}
    }
    str[13] = '\0';
}

static int is_right(const unsigned char*str)
{
    int i = 0;
    int sum1 = 0;
    int sum2 = 0;

    for(i = 1; i < 12; i+= 2)
    {
	sum1 += (str[i] - '0');
    }

    for(i = 0; i < 12; i+= 2)
    {
	sum2 += (str[i] - '0');
    }

    sum1 = sum1 * 3 + sum2;

    sum2 = ((sum1 / 10) + 1) * 10 - sum1;
    
    if(sum2 == (str[12] - '0'))
    {
	return 1;
    }
    else
    {
	return 0;
    }
}
UINT8* Identify_charbar(const UINT8*tmp,int width,int height)
{ 
    int i = 0;
    int j = 0;
    int **matrix = NULL;
    int *line = NULL;
    unsigned char* str = (unsigned char*)malloc(sizeof(char) * 14);

    assert(tmp);
    assert(width > 0 && height > 0);

    init(); 
    matrix = (int**) malloc(height*sizeof(int*)); 
    assert(matrix);
    
    //printf("width:%d,height:%d\n",width,height);

    for(i = 0; i < height; i++)
    {
	matrix[i] = (int*)malloc((MATRIX_W)*sizeof(int));
	assert(matrix[i]);
    }
#if 0 
    for(i = 0; i < height; i++)
    {
	for(j = 0; j < width; j++)
	{
	    printf("%u ",tmp[i*width+j]);
	}
	printf("\n");
    }
#endif
    line = matrix_init(tmp,width,height,matrix);
    if(line == NULL)
    {
	printf("此条形码无效!\n");
	return NULL;
    }
#if 0
    for(i = 0;i < (MATRIX_W - 1); i++ )
    {
	printf("line[%d]:%d\n",i,line[i]);
    }
#endif


    for(i = 0; i < height;i++)
    {
	free(matrix[i]);
    }
    free(matrix);
    
    Normalize(line,str);

    free(line);
    
    if(is_right(str))
    {
	printf("条形码是%s\n",str);
	return str;
    }
    else
    {
	printf("条形码是%s\n",str);
	printf("识别条形码失败\n");
	return NULL;
    }

}
