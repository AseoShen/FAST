#include "fast.h"

void fast_accel(DTYPE* img_in, int threshold, DTYPE* img_out, int rows, int cols)
{

#pragma HLS INTERFACE m_axi port=img_in offset=slave depth=16384
#pragma HLS INTERFACE m_axi port=img_out offset=slave depth=16384
#pragma HLS INTERFACE s_axilite port=threshold bundle=CTRL
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

	//以左上角为原点，向右为x正方向（cols），向下为y正方向（rows）
    for (int y = RADIUS; y < rows-RADIUS; y++)
#pragma HLS PIPELINE II=20
    	for (int x = RADIUS; x < cols-RADIUS; x++)
    	{
#pragma HLS PIPELINE II=20
    		int count = check_pre(img_in, threshold, x, y, rows, cols);	//计数器
    		//预判断结果
    		if (count < 3)
    		{
    			img_out[y*cols+x] = 0;
    			continue;
    		}
    		//正式判断
    		count += check_for(img_in, threshold, x, y, rows, cols);
    		if (count >= 12)
    			img_out[y*cols+x] = 255;
    		else
    			img_out[y*cols+x] = 0;
    	}
}

//预判断函数，返回查询到的点数
int check_pre(DTYPE* img_in, int threshold, int x, int y, int rows, int cols)
{
#pragma HLS INLINE
	DTYPE center = img_in[y*cols+x];
	int count_pre = check_thres(center, img_in[(y-3)*cols+x], threshold);	//P1
	count_pre += check_thres(center, img_in[y*cols+x+3], threshold);		//P5
	count_pre += check_thres(center, img_in[(y+3)*cols+x], threshold);		//P9
	count_pre += check_thres(center, img_in[y*cols+x-3], threshold);		//P13
	return count_pre;
}

//正式判断函数
int check_for(DTYPE* img_in, int threshold, int x, int y, int rows, int cols)
{
#pragma HLS INLINE
	DTYPE center = img_in[y*cols+x];
	int count_for = check_thres(center, img_in[(y-3)*cols+x+1], threshold);	//P2
	count_for += check_thres(center, img_in[(y-2)*cols+x+2], threshold);	//P3
	count_for += check_thres(center, img_in[(y-1)*cols+x+3], threshold);	//P4
	count_for += check_thres(center, img_in[(y+1)*cols+x+3], threshold);	//P6
	count_for += check_thres(center, img_in[(y+2)*cols+x+2], threshold);	//P7
	count_for += check_thres(center, img_in[(y+3)*cols+x+1], threshold);	//P8
	count_for += check_thres(center, img_in[(y+3)*cols+x-1], threshold);	//P10
	count_for += check_thres(center, img_in[(y+2)*cols+x-2], threshold);	//P11
	count_for += check_thres(center, img_in[(y+1)*cols+x-3], threshold);	//P12
	count_for += check_thres(center, img_in[(y-1)*cols+x-3], threshold);	//P14
	count_for += check_thres(center, img_in[(y-2)*cols+x-2], threshold);	//P15
	count_for += check_thres(center, img_in[(y-3)*cols+x-1], threshold);	//P16
	return count_for;
}

int check_thres(DTYPE center, DTYPE point, int threshold)
{
#pragma HLS INLINE
	if (point - center > threshold || center - point > threshold)
		return 1;
	else
		return 0;
}
