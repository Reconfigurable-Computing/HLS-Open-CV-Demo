/******************************************************************************
*
* Copyright (C) 2014 - 2018 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*
*
******************************************************************************/
/*****************************************************************************/
/**
 *
 * @file vdma.c
 *
 * This file comprises sample application to  usage of VDMA APi's in vdma_api.c.
 *  .
 *
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 4.0   adk  11/26/15 First release
 * 4.1   adk  01/07/16 Updated DDR base address for Ultrascale (CR 799532) and
 *		       removed the defines for S6/V6.
 *       ms   04/05/17 Modified Comment lines in functions to
 *                     recognize it as documentation block for doxygen
 *                     generation of examples.
 ****************************************************************************/

/*** Include file ***/
#include "xparameters.h"
#include "xstatus.h"
#include "xil_exception.h"
#include "xil_assert.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "xil_io.h"
#include <stdio.h>
#include "xil_cache.h"
#include "xsobel.h"
#include "ff.h"
#include "bmp.h"
#include "vdma_api.h"

#define MEMORY_BASE		0x01000000

/*** Global Variables ***/
unsigned int srcBuffer = (MEMORY_BASE  + 0x1000000);
unsigned int destBuffer = (MEMORY_BASE + 0x3000000);
//
static FATFS fatfs;

int SD_Init()
{
    FRESULT rc;
    //
    rc = f_mount(&fatfs,"",0);
    if(rc)
    {
        xil_printf("ERROR : f_mount returned %d\r\n",rc);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

void img_expand(unsigned char* img,unsigned char* expand_img,int size){
	//since the datawidth of sobel ip is 32bit,every pixel will be packed into 4byte with 0xff
	for(int i=0;i<size;i++){
		expand_img[4*i+0]=img[3*i+0];
		expand_img[4*i+1]=img[3*i+1];
		expand_img[4*i+2]=img[3*i+2];
		expand_img[4*i+3]=0xff;
	}
}

void img_extract(unsigned char* ori_img,unsigned char* img,int h,int w){
	//for original hxw picture, every line need to be 4 byte aligned,this function is used to discard the invalid byte.
	int hstride=((w*3+3)/4)*4;
	for(int i=0;i<h;i++)
	   for(int j=0;j<w;j++){
        img[3*(i*w+j)+0]=ori_img[i*hstride+3*j+0];
        img[3*(i*w+j)+1]=ori_img[i*hstride+3*j+1];
        img[3*(i*w+j)+2]=ori_img[i*hstride+3*j+2];
	}
}

void img_unextract(unsigned char* ori_img,unsigned char* img,int h,int w){
	//opposite of img_expand
	int hstride=(w*3+3)/4*4;
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++){
			img[i*hstride+j*3+0]=ori_img[3*(i*w+j)+0];
			img[i*hstride+j*3+1]=ori_img[3*(i*w+j)+1];
			img[i*hstride+j*3+2]=ori_img[3*(i*w+j)+2];
		}
}

int main(){
	int Status;
	int image_size,h,w;
	const char* input_image="lena384x256.bmp";
	const char* output_image="out1.bmp";
	BITMAPFILEHEADER bmp_file;
	BITMAPINFOHEADER bmp_info;
	FIL fil;
	UINT bw;
	XAxiVdma InstancePtr;
    XSobel hls_sobel;
	//disable dcache and init sd card
	Xil_DCacheDisable();
	SD_Init();
	//read input
	f_open(&fil,input_image,FA_READ);
	//read image information
	f_read(&fil,&bmp_file,14,&bw);
	f_read(&fil,&bmp_info,40,&bw);
	image_size=bmp_info.biSizeImage;
	h=bmp_info.biHeight;
    w=bmp_info.biWidth;
    printf("biSizeImage=%d,biHeight=%d,biWidth=%d\n",image_size,h,w);
    //define buffer
    unsigned char* ori_image=(unsigned char*)malloc(image_size);
    unsigned char* image=(unsigned char*)malloc(h*w*3);
    unsigned char* image_expand=(unsigned char*)malloc(h*w*4);
    unsigned char* image_sobel=(unsigned char*)malloc(h*w*3);
    unsigned char* image_sobel_expand=(unsigned char*)malloc(image_size);
    //read image data
    f_lseek(&fil,bmp_file.bfOffBits);                                 //file pointer
    f_read(&fil,ori_image,bmp_info.biSizeImage,&bw);               //read image data
	//preprocess
    img_extract(ori_image,image,h,w);
	img_expand(image,image_expand,h*w);
	for(int i=0;i<h*w*4;i++){
		Xil_Out8(srcBuffer+i,(u8)image_expand[i]);
	}
	//init and run sobel ip core
	XSobel_Initialize(&hls_sobel,0);
	XSobel_Set_rows(&hls_sobel, (u32)h);
	XSobel_Set_cols(&hls_sobel, (u32)w);
	XSobel_Start(&hls_sobel);
    //init and run axi vdma
	Status = run_vdma(&InstancePtr, 0, w, h, srcBuffer, destBuffer, 1, 0);
    //
	if(Status != XST_SUCCESS) {
		xil_printf("Transfer of frames failed with error = %d\r\n",Status);
		return XST_FAILURE;
	}
	while(XSobel_IsDone(&hls_sobel));
	//
	for(int i=0;i<h*w;i++){
		u32 x=Xil_In32(destBuffer+i*4);
		image_sobel[i*3+0]=x&0xff;
		image_sobel[i*3+1]=(x>>8)&0xff;
		image_sobel[i*3+2]=(x>>16)&0xff;
    }
    img_unextract(image_sobel,image_sobel_expand,h,w);
    //****************************************write sobel image to sd card**************************************************
    //set bmp_file
    bmp_file.bfType=0x4d42;
    bmp_file.bfSize=54+image_size;
    bmp_file.bfReserved1=0;
    bmp_file.bfReserved2=0;
    bmp_file.bfOffBits=54;
    //set bmp_info
    bmp_info.biSize=40;
    bmp_info.biWidth=w;
    bmp_info.biHeight=h;
    bmp_info.biPlanes=1;
    bmp_info.biBitCount=24;
    bmp_info.biCompression=0;
    bmp_info.biSizeImage=image_size;
    bmp_info.biXPelsPerMeter=0;
    bmp_info.biYPelsPerMeter=0;
    bmp_info.biClrImportant=0;
    bmp_info.biClrUsed=0;
    //write bmp
    f_open(&fil,output_image,FA_CREATE_ALWAYS | FA_WRITE);
    f_write(&fil,&bmp_file,14,&bw);
    f_write(&fil,&bmp_info,40,&bw);
    f_write(&fil,image_sobel_expand,bmp_info.biSizeImage,&bw);
    f_close(&fil);
    //free
	free(ori_image);
	free(image);
	free(image_expand);
	free(image_sobel);
	free(image_sobel_expand);
    return 0;
}
