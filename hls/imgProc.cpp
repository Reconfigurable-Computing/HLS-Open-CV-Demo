#include "imgProc.h"

void sobel(AXI_STREAM& INPUT_STREAM,
		   AXI_STREAM& OUTPUT_STREAM,
	       int rows,int cols)
{
#pragma HLS INTERFACE axis port = INPUT_STREAM
#pragma HLS INTERFACE axis port = OUTPUT_STREAM
#pragma HLS INTERFACE s_axilite port = rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port = cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS dataflow

	//hls::mat
	RGB_IMAGE img_0(rows,cols);
	GRAY_IMAGE img_1(rows,cols);
	GRAY_IMAGE img_2(rows,cols);
	RGB_IMAGE img_3(rows,cols);
	//
	hls::AXIvideo2Mat(INPUT_STREAM,img_0);
	//
	hls::CvtColor<HLS_RGB2GRAY,HLS_8UC3,HLS_8UC1>(img_0,img_1);
	//
	hls::Sobel<1,1,5>(img_1,img_2);
	//
	hls::CvtColor<HLS_GRAY2RGB,HLS_8UC1,HLS_8UC3>(img_2,img_3);
	//
	hls::Mat2AXIvideo(img_3,OUTPUT_STREAM);
}

void rgb2gray(AXI_STREAM& INPUT_STREAM,
			  AXI_STREAM& OUTPUT_STREAM,
			  int rows,int cols)
{
#pragma HLS INTERFACE axis port = INPUT_STREAM
#pragma HLS INTERFACE axis port = OUTPUT_STREAM
#pragma HLS INTERFACE s_axilite port = rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port = cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS dataflow

	//hls::mat
	RGB_IMAGE img_0(rows,cols);
	GRAY_IMAGE img_1(rows,cols);
	RGB_IMAGE img_2(rows,cols);
    //
	hls::AXIvideo2Mat(INPUT_STREAM,img_0);
	hls::CvtColor<HLS_RGB2GRAY,HLS_8UC3,HLS_8UC1>(img_0,img_1);
	hls::CvtColor<HLS_GRAY2RGB,HLS_8UC1,HLS_8UC3>(img_1,img_2);
	hls::Mat2AXIvideo(img_2,OUTPUT_STREAM);
}

void gaussian(AXI_STREAM& INPUT_STREAM,
		      AXI_STREAM& OUTPUT_STREAM,
		      int rows,int cols)
{

#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE axis port=OUTPUT_STREAM
#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS DATAFLOW

	RGB_IMAGE img_0(rows, cols);
	GRAY_IMAGE img_1(rows, cols);
	GRAY_IMAGE img_2(rows, cols);
	RGB_IMAGE img_3(rows, cols);

	hls::AXIvideo2Mat(INPUT_STREAM, img_0);
	hls::CvtColor<HLS_RGB2GRAY, HLS_8UC3, HLS_8UC1>(img_0, img_1);
	hls::GaussianBlur<3,3>(img_1, img_2);
	hls::CvtColor<HLS_GRAY2RGB, HLS_8UC1, HLS_8UC3>(img_2, img_3);
	hls::Mat2AXIvideo(img_3, OUTPUT_STREAM);
}

void binary(AXI_STREAM& INPUT_STREAM,
		    AXI_STREAM& OUTPUT_STREAM,
	    	int rows,int cols)
{
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE axis port=OUTPUT_STREAM
#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS DATAFLOW

	RGB_IMAGE img_0(rows, cols);
	GRAY_IMAGE img_1(rows, cols);
	GRAY_IMAGE img_2(rows, cols);
	RGB_IMAGE img_3(rows, cols);

	hls::AXIvideo2Mat(INPUT_STREAM, img_0);
	hls::CvtColor<HLS_RGB2GRAY, HLS_8UC3, HLS_8UC1>(img_0, img_1);
	hls::Threshold(img_1, img_2, 150, 255, HLS_THRESH_BINARY);
	hls::CvtColor<HLS_GRAY2RGB, HLS_8UC1, HLS_8UC3>(img_2, img_3);
	hls::Mat2AXIvideo(img_3, OUTPUT_STREAM);
}

void dilate_erode(AXI_STREAM& INPUT_STREAM,
		          AXI_STREAM& OUTPUT_STREAM,
		          int rows,int cols)
{
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE axis port=OUTPUT_STREAM
#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS DATAFLOW

	RGB_IMAGE img_0(rows, cols);
	GRAY_IMAGE img_1(rows, cols);
	GRAY_IMAGE img_2(rows, cols);
	GRAY_IMAGE img_3(rows, cols);
	RGB_IMAGE img_4(rows, cols);

	hls::AXIvideo2Mat(INPUT_STREAM, img_0);
	hls::CvtColor<HLS_RGB2GRAY, HLS_8UC3, HLS_8UC1>(img_0, img_1);
	hls::Dilate(img_1, img_2);
	hls::Erode(img_2, img_3);
	hls::CvtColor<HLS_GRAY2RGB, HLS_8UC1, HLS_8UC3>(img_3, img_4);
	hls::Mat2AXIvideo(img_4, OUTPUT_STREAM);
}

//void fast_corner(AXI_STREAM& INPUT_STREAM,AXI_STREAM& OUTPUT_STREAM,int rows,int cols)
//{
//#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
//#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
//#pragma HLS INTERFACE axis port=OUTPUT_STREAM
//#pragma HLS INTERFACE axis port=INPUT_STREAM
//    //
//	int threhold=20;
//	RGB_IMAGE _src(rows,cols);
//	RGB_IMAGE _dst(rows,cols);
//	RGB_IMAGE src0(rows,cols);
//	RGB_IMAGE src1(rows,cols);
//	GRAY_IMAGE mask(rows,cols);
//	GRAY_IMAGE dmask(rows,cols);
//	GRAY_IMAGE gray(rows,cols);
//	//
//#pragma HLS dataflow
//#pragma HLS STREAM variable=src1.data_stream depth=20000 dim=1
//    //
//	hls::AXIvideo2Mat(INPUT_STREAM, _src);
//	hls::Scalar<3,unsigned char> color(255,0,0);
//	hls::Duplicate(_src,src0,src1);
//	hls::CvtColor<HLS_BGR2GRAY>(src0,gray);
//	hls::FASTX(gray,mask,threhold,true);
//	hls::Dilate(mask,dmask);//做膨胀处理
//	hls::PaintMask(src1,dmask,_dst,color);
//	hls::Mat2AXIvideo(_dst,OUTPUT_STREAM);
//}
//
void rgb_to_gray(RGB_IMAGE & imgIn, RGB_IMAGE & imgOut_3C, GRAY_IMAGE & imgOut_1C,int rows,int cols){
	PIXEL_RGB 	pixIn;
	PIXEL_RGB 	pixOut_3C;
	PIXEL_GRAY	pixOut_1C;

	for(int idxRow = 0; idxRow < rows; idxRow++){
		for(int idxCol = 0; idxCol < cols; idxCol++){
			//输入图像是三通道的RGB图像
			imgIn >> pixIn;
			unsigned short R = pixIn.val[0];
			unsigned short G = pixIn.val[1];
			unsigned short B = pixIn.val[2];
			//输出三�?�道图像，各个�?�道都是灰度值，�?终显示出来的图像是灰度图�?
			pixOut_3C.val[0] = (unsigned char) ((R*76 + G*150 + B*30) >> 8);
			pixOut_3C.val[1] = (unsigned char) ((R*76 + G*150 + B*30) >> 8);
			pixOut_3C.val[2] = (unsigned char) ((R*76 + G*150 + B*30) >> 8);
			//输出单�?�道图像，只有一个�?�道，为灰度图像
			pixOut_1C.val[0] = (unsigned char) ((R*76 + G*150 + B*30) >> 8);
			imgOut_3C << pixOut_3C;
			imgOut_1C << pixOut_1C;
		}
	}
}

void fast_corner(AXI_STREAM & INPUT_STREAM,AXI_STREAM & OUTPUT_STREAM,int rows,int cols){
#pragma HLS INTERFACE axis  port=OUTPUT_STREAM
#pragma HLS INTERFACE axis  port=INPUT_STREAM
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS INTERFACE s_axilite port=rows bundle=CTRL
#pragma HLS INTERFACE s_axilite port=cols bundle=CTRL

	RGB_IMAGE img_0(rows,cols);
	RGB_IMAGE img_1(rows,cols);
	GRAY_IMAGE img_2(rows,cols);
	RGB_IMAGE img_3(rows,cols);
	GRAY_IMAGE mask(rows,cols);
	GRAY_IMAGE dmask(rows,cols);

#pragma HLS dataflow
#pragma HLS stream depth=20000 variable=img_1_.data_stream
	hls::AXIvideo2Mat(INPUT_STREAM, img_0);
	PIXEL_RGB color(255,255,0);
	rgb_to_gray(img_0, img_1, img_2, rows, cols);
	hls::FASTX(img_2, mask, 20, true);                   //快�?�角点检�?
	hls::Dilate(mask, dmask);                            //对检测出来的点进行膨�?
	hls::PaintMask(img_1, dmask, img_3, color);          //将膨�?后的点添加到图像�?
	hls::Mat2AXIvideo(img_3, OUTPUT_STREAM);
}




