#include "xaxivdma.h"
#include "xparameters.h"
#include "xil_exception.h"

int run_vdma(XAxiVdma* InstancePtr, int DeviceId, int hsize,int vsize, int src_buf_base_addr,int dest_buf_base_addr, int number_frame_count,int enable_frm_cnt_intr);