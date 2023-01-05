#include<stdio.h>

#pragma pack(1)

typedef struct{
    unsigned short bfType;                            //�ļ�����,����ֵΪ0x4D42
    int bfSize;                                        //�ļ���С�����ֽ�Ϊ��λ
    unsigned short bfReserved1;                       //����ֵ,����Ϊ0
    unsigned short bfReserved2;                       //����ֵ,����Ϊ0
    int bfOffBits;                                     //���ļ���ͷ������ͼ�����ݵ��ֽ�ƫ�����������ļ�ͷ��14byte��+λͼ��Ϣͷ��40byte��+��ɫ�壩
}BITMAPFILEHEADER;

typedef struct{
   unsigned int biSize;                           //λͼ��Ϣͷ�Ĵ�С,��Ϊ40�ֽ�
   int biWidth;                                    //λͼ���ȣ���λΪ����
   int biHeight;                                   //λͼ�߶ȣ���λΪ����
   unsigned short biPlanes;                      //λͼ��ƽ����������Ϊ1
   unsigned short biBitCount;                    //����λ������1��4��8��16��24��32��
   unsigned int biCompression;                   //λͼѹ�����ͣ�0Ϊ��ѹ����1ΪBI_RLE8��2ΪBI_RLE4(30-33)
   unsigned int    biSizeImage;                  //ͼ�����ݲ��ִ�С,����bfSize-bfOffBits (34-37)
   int     biXPelsPerMeter;                       // ������/�ױ�ʾ��ˮƽ�ֱ���
   int     biYPelsPerMeter;                       // ������/�ױ�ʾ�Ĵ�ֱ�ֱ���
   unsigned int    biClrUsed;                    // ��ɫ���е���ɫ������,0ΪͼƬ�е�ɫ��
   unsigned int    biClrImportant;               // ��Ҫ��ɫ��������Ŀ��0��ʾ����Ҫ(50-53)
}BITMAPINFOHEADER;

typedef struct
{
    unsigned char rgbBlue;  //��ɫ��������
    unsigned char rgbGreen; //��ɫ��������
    unsigned char rgbRed;   //��ɫ��������
    unsigned char rgbReserved;
}RGBQUAD;

void showbmp(BITMAPFILEHEADER bmp){
   printf("file type:%x\n",bmp.bfType);
   printf("file size=%d\n",bmp.bfSize);
   printf("reserved1=%d\n",bmp.bfReserved1);
   printf("reserved2=%d\n",bmp.bfReserved2);
   printf("data offset=%d\n",bmp.bfOffBits);
}

void showbmpinfo(BITMAPINFOHEADER info){
   printf("struct size=%d\n",info.biSize);
   printf("image width=%d\n",info.biWidth);
   printf("image height=%d\n",info.biHeight);
   printf("biPlanes=%d\n",info.biPlanes);
   printf("image bircount=%d\n",info.biBitCount);
   printf("biCompression:%d\n",info.biCompression);
   printf("image size:%d\n",info.biSizeImage);
   printf("biXPelsPerMeter:%d\n",info.biXPelsPerMeter);
   printf("biYPelsPerMeter:%d\n",info.biYPelsPerMeter);
   printf("biClrUsed:%d\n",info.biClrUsed);
   printf("biClrImportant:%d\n",info.biClrImportant);
}
