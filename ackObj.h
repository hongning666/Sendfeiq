#ifndef __ACKOBJ_H_
#define __ACKOBJ_H_
/*宏定义*/
#define DATASIZE 256
//攻击程序对象
typedef struct _TackObj{
	int sockfd;
	char databuf[DATASIZE];
	char dstip[200][32];
}ackObj;
typedef ackObj *packObj;
packObj g_ackObj;

/*
函数名称:ackObj_init 
函数功能:初始化g_ackObj对象
函数参数:void
函数返回值:成功返回0
			失败返回-1
 */
int ackObj_init();
/*
函数名称:ackObj_create 
函数功能:创建对象 
函数参数:dstip目标ip
		 data要发送的数据
返回值:成功返回0
		失败返回-1
 */
int ackObj_create();

/*
函数名称:ackObj_handle
函数功能:逻辑功能处理
参数：
返回值:
   */
void ackObj_handle();

void ackObj_release();

#endif
