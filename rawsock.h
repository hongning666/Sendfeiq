#ifndef __RAWSOCK_H_
#define __RAWSOCK_H_
/*头文件*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netdb.h>

/*宏定义*/
#define UDPTYPE 0x1000
#define IPHDRLEN sizeof(struct ip)
#define UDPHDRLEN sizeof(struct udphdr)

/*函数声明*/
/*
函数名：rawsock_create
功能:创建一个原始套接字
参数:void
返回值:成功返回套接字
	   失败返回-1
*/
int rawsock_create();

/*
函数名：rawsock_close
功能：关闭原始套接字
参数：rawfd 原始套机字描述符
返回值：void
*/
void rawsock_close(int rawfd);

/*
函数名:udpdata_create
功能:构造udp数据包
参数:data将要发送的数据
	 datalen数据长度
返回值:void
*/
void udpdata_create(char *buf,const char *data,char *dstip,struct sockaddr_in *dstaddr);
/*
函数名:iphdr_create
功能:封装ip头部
参数:ip_header
返回值:void
*/
static void iphdr_create(struct ip *ip_header,char *dstip,int type,int iplen,struct sockaddr_in *dstaddr);
/*
函数名:udphdr_create
功能:封装udp头部
参数:udp_header
返回值:void
*/
static void udphdr_create(struct udphdr *udp_header,int udplen);

#endif
