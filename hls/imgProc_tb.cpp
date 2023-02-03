#include "imgProc.h"
#include "hls_opencv.h"
#include<stdio.h>

#define INPUT_IMAGE "C:\\Users\\DELL\\Desktop\\lena.bmp"
//#define INPUT_IMAGE "C:\\Users\\DELL\\Desktop\\HLS-Open-CV-Demo\\picture\\lena289x289.bmp"
#define OUTPUT_IMAGE "C:\\Users\\DELL\\Desktop\\HLS-Open-CV-Demo\\picture\\lena_gray.bmp"

int main(void)
{
	//��ȡͼ������
	IplImage* src = cvLoadImage(INPUT_IMAGE);
	IplImage* dst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);

	//ʹ��HLS����д���
	AXI_STREAM src_axi,dst_axi;
	IplImage2AXIvideo(src,src_axi);
	//sobel(src_axi,dst_axi,src->height,src->width);
	//rgb2gray(src_axi,dst_axi,src->height,src->width);
	//gaussian(src_axi,dst_axi,src->height,src->width);
	//binary(src_axi,dst_axi,src->height,src->width);
	//dilate_erode(src_axi,dst_axi,src->height,src->width);
	fast_corner(src_axi,dst_axi,src->height,src->width);
	//
	AXIvideo2IplImage(dst_axi,dst);

	//����ͼ��
	cvSaveImage(OUTPUT_IMAGE,dst);

	//��ʾͼ��
	cvShowImage(INPUT_IMAGE,src);
	cvShowImage(OUTPUT_IMAGE,dst);

	//�ȴ��û����¼����ϵ���һ����
	cv::waitKey(0);
}




