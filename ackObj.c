#include "ackObj.h"
#include "rawsock.h"

int ackObj_init(){
	g_ackObj = (packObj)malloc(sizeof(ackObj));
	if(g_ackObj == NULL)
		return -1;
	memset(g_ackObj,0,sizeof(ackObj));
	return 0;
}

int ackObj_create(){
	int i;
	g_ackObj->sockfd = rawsock_create();
	if(g_ackObj->sockfd == -1)
		return -1;
	for(i = 0;i<200;++i)
		sprintf(g_ackObj->dstip[i],"176.4.11.%d",i+1);
	return 0;
}

void ackObj_handle(const char *data){//为了方便用缺省参数方式把数据写死了
	int i;
	struct sockaddr_in target;
	target.sin_family = AF_INET;
	target.sin_port = htons(2425);
   while(1)
   {
		
		for(i = 0;i<200;++i)
		{
			udpdata_create(g_ackObj->databuf,data,g_ackObj->dstip[i],&target);
			sendto(g_ackObj->sockfd,g_ackObj->databuf,IPHDRLEN+UDPHDRLEN+strlen(data),0,(struct sockaddr*)&target,sizeof(target));
		}
		sleep(1);
   }
}

void ackObj_release(){
	free(g_ackObj);
	g_ackObj = NULL;
}
