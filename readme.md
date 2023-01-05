**实验平台**：Zedboard(XC7Z020-CLG484-1)  
**开发工具**：Vivado2019.2开发套件  
（由于jpg、png等图片格式涉及图像压缩算法，因此在zynq平台读取此类图片较为困难，故目前仅支持bmp图片的读取）
# bmp图片格式介绍
　　BMP(全称Bitmap)是Window操作系统中的标准图像文件格式，可以分成两类：设备相关位图(DDB)和设备无关位图(DIB)，使用非常广。它采用位映射存储格式，除了图像深度可选以外，不采用其他任何压缩，因此，BMP文件所占用的空间很大。BMP文件的图像深度可选1bit、4bit、8bit及24bit。BMP文件存储数据时，图像的扫描方式是按**从左到右、从下到上**的顺序。 由于BMP文件格式是Windows环境中交换与图有关的数据的一种标准，因此在Windows环境中运行的图形图像软件都支持BMP图像格式。  
BMP文件格式由文件头和原始位图数据Raw Bitmap Data构成（详见https://en.wikipedia.org/wiki/BMP_file_format）。
## 文件头
位图格式的文件头长度可变，而且其中参数繁多。但通常情况下bmp格式图片的文件头长度均为**54字节**，其中包括**14字节的Bitmap文件头**以及**40字节的DIB（Device Independent Bitmap）数据头，或称位图信息数据头（BItmap Information Header）**。
1. 位图文件头 Bitmap File Header (14 bytes)
![image](https://user-images.githubusercontent.com/44521731/210231242-c90455b3-8b73-4f13-a57d-730881db7daa.png)

2. 位图信息数据头 DIB Header (54 bytes)
![image](https://user-images.githubusercontent.com/44521731/210231308-c60fd08d-32ab-4ffc-a438-f986d6fbc0bd.png)
## 原始位图数据Raw Bitmap Data
图像数据块从文件头中起始偏移量字段所指出的位置开始，其中存放着位图图像的数据，数据格式由图像参数信息块中的压缩方式选项的取值决定。操作图像数据块时，需要注意：
当压缩方式为RGB时，图像数据块**以“行”为单位双字（4字节）对齐**，如1行像素的字节数不为4的倍数，则进行填充，直至4字节对齐为止（一般填充0）。
# Demo流程
## HLS开发
1. 创建HLS工程，选择板子型号为XC7Z020-CLG484-1，并添加hls文件夹内的源文件及测试平台文件。
2. 修改宏INPUT_IMAGE和宏OUTPUT_IMAGE，分别表示待处理图片的文件路径以及图片输出路径。
3. 点击Run C Simulation，进行C仿真。
4. 点击Run C Synthesis，进行C综合。
5. 点击Run C/RTL Cosimulation，进行C/RTL协同仿真。
6. 点击Export RTL as IP，将综合后的代码导出为IP。
## Block Design
1. 创建Vivado工程，选择板子型号为XC7Z020-CLG484-1。
2. 点击IP Catalog，添加HLS导出IP的路径。
3. 点击Create Block Design，分别添加HLS导出的IP（名为sobel）、AXI VDMA IP核以及ZYNQ IP核。如下图所示：
![{9 35TN8QKG`QEHJJNYU`F4](https://user-images.githubusercontent.com/44521731/210229559-780636de-7af3-44f0-a04e-cbbd1324578b.png)

4. 配置IP核  
**sobel ip**：无需配置。  
**axi vdma ip**：将Frame Buffers设置为1，其余保持默认即可。  
**zynq ip**：包括勾选1个AXI GP接口，2个AXI HP接口，设置PL端时钟频率，勾选UART和SD，配置DDR型号（因开发板型号不同而不同）。
5. 手动连接AXI STREAM接口，其余自动连线即可，连线完成后，进行Validate Design，最终的硬件系统如下所示
![%A7~`}M_OHTW(P57PP$S L](https://user-images.githubusercontent.com/44521731/210229606-d95beed7-d2eb-47e1-bf42-3fd5498f1c6e.png)
6. 右键block design文件，点击Create HDL Wrapper。
7. 点击Generate Bitstream，进行综合、布局布线、生成比特流。
8. File-->Export-->Export Hardware，勾选Include bitstream，然后点击OK。
9. Tool-->Launch Vitis。
## SDK开发
1. Launch Vitis后，选择Workspace路径，点击Launch。
2. **创建平台**  
Create Platform Project，输入Project Name，next，next，点击Browse选择vivado导出的.xsa硬件描述文件，最后点击Finish。
3. 选择zynq_fsbl下的Board Support Package，点击Modify BSP Settings，勾选xilffs，并将xilffs设置中的use_lfn的修改为1，以支持长文件名的读取。对standalone on ps7_cortexa9_0下的Board Support Package进行相同的操作。
4. **创建SDK工程**  
点击File-->New-->Application Project，填写Project name，next，next，next，选择Empty Application，点击Finish。
5. 将sdk文件夹下的所有文件拷贝至创建工程的src文件夹下。
6. 双击lscript.ld，修改堆栈大小。如将Stack Size和Heap Size均调节为0x2000000。
7. 右键工程，点击Build Project。连接开发板，打开串口调试助手（波特率选择115200）。将bmp图片存入SD卡，插到FPGA开发板上。
8. **烧写程序**。右键工程，点击Run As-->Run Configuartions，双击Single Application Debug，点击Run。
9. 可通过修改const char\* input_image和const char\* output_image来改变待测试图片和输出图片的文件名。
10. 运行完成后，拔出SD卡，在PC上通过读卡器查看结果。

![XV5E3K_HP2LDY3`KSK(V5P](https://user-images.githubusercontent.com/44521731/210229958-a8eedc2f-e1ce-46d4-9a09-06f506812481.png)
