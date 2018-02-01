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

void  sendudp(int skfd);


int main(int argc,char *argv[])
{
	int     connfd;  
      
    const int on=1;
    
 
    //!> 建立套接字  并且指定为IPPROTO_TCP的原始套接字
    if( ( connfd = socket( AF_INET, SOCK_RAW, IPPROTO_UDP) ) == -1 )  
    {  
        printf("Socket Error...\n");  
        exit( EXIT_FAILURE );  
    }  

  //开启IP头部的更改，由用户自行封装IP报文  模板
    if( ( setsockopt(connfd, IPPROTO_IP,IP_HDRINCL,&on, sizeof(on)) ) == -1 ) 
    {
          printf("setsockopt Error...\n");  
          exit(1);
    }

  //必须获得管理员权限 才能进行raw socket
	setuid(getpid());
	
	sendudp(connfd);
	return 0;
}
void  sendudp(int skfd)
{
   int ip_len,udp_len;
   int datalen;
   int i;
   char buf[256]={0};
   char desip[200][32] = {0};
   struct ip *ip;
   struct udphdr *udp_header;
   
   struct sockaddr_in target;
    //!> 套接字信息  
    bzero(&target, sizeof(struct sockaddr_in));  
    target.sin_family = AF_INET;  
    target.sin_port = htons(2425);  
   for(i = 0;i<200;++i)
   {
		sprintf(desip[i],"176.4.14.%d",i);
   }
   datalen = strlen("1:100:T.T:PC:32:hello!nihao");
  //设置当前进行SYN包的数据长度
   ip_len=sizeof(struct ip) + sizeof(struct udphdr)+datalen;
   //对IP头进行填充
   //给IP头部进行内存空间的分配
   ip=(struct ip*)buf;
   //ip头部填充
   ip->ip_v=IPVERSION; //版本号
   ip->ip_hl=sizeof(struct ip)>>2; //ip 头部长度
   ip->ip_tos=0;
   ip->ip_len=htons(ip_len);
   ip->ip_id=0;
   ip->ip_off=0;
   ip->ip_ttl=MAXTTL;
   ip->ip_p=IPPROTO_UDP;
   ip->ip_sum=0;
     
   udp_len = sizeof(struct udphdr) + datalen;
   udp_header = (struct udphdr *)(buf+sizeof(struct ip));
   udp_header->source = htons(2425);
   udp_header->dest = htons(2425);
   /* 这里的长度是整个UDP报文 */
   udp_header->len = htons(udp_len);
   udp_header->check = 0;
   memcpy(buf+sizeof(struct ip)+sizeof(struct udphdr),"1:100:T.T:PC:32:hello!nihao",datalen);
   while(1)
   {
		
		for(i = 0;i<200;++i)
		{
   			ip->ip_src.s_addr = random();
			inet_pton(AF_INET,desip[i],&target.sin_addr);
			ip->ip_dst = target.sin_addr;
			int ret = sendto(skfd,buf,ip_len,0,(struct sockaddr*)&target,sizeof(target));
			if(ret == -1)	
				perror("sendto");
			sleep(1);
		}
		sleep(1);
   }
   close(skfd);
}
