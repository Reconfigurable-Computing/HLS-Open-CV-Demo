#ifndef _OV5640_SOBEL_H
#define _OV5640_SOBEL_H

#include "hls_video.h"
#include <iostream>
using namespace std;

#define MAX_HEIGHT 1080    //ͼ�����߶�
#define MAX_WIDTH  1920    //ͼ�������

typedef hls::stream<ap_axiu<32,1,1,1> > AXI_STREAM;
typedef hls::Mat<MAX_HEIGHT,MAX_WIDTH,HLS_8UC3> RGB_IMAGE;
typedef hls::Mat<MAX_HEIGHT,MAX_WIDTH,HLS_8UC1> GRAY_IMAGE;

void sobel(AXI_STREAM& INPUT_STREAM,
           AXI_STREAM& OUTPUT_STREAM,
           int rows,
           int cols
          );

#endif

