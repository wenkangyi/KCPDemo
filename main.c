#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ikcp.h"
#include "udp_server.h"
#include "udp_client.h"

#ifdef __unix
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif

IUINT32 conv = 0;
void *user = NULL;
ikcpcb *kcp = NULL;
int sockfd = -1;
int flag = 0;//0--client    1--server




/* get system time */
static inline void itimeofday(long *sec, long *usec)
{
	#if defined(__unix)
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) *sec = time.tv_sec;
	if (usec) *usec = time.tv_usec;
	#else
	static long mode = 0, addsec = 0;
	BOOL retval;
	static IINT64 freq = 1;
	IINT64 qpc;
	if (mode == 0) {
		retval = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		freq = (freq == 0)? 1 : freq;
		retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
		addsec = (long)time(NULL);
		addsec = addsec - (long)((qpc / freq) & 0x7fffffff);
		mode = 1;
	}
	retval = QueryPerformanceCounter((LARGE_INTEGER*)&qpc);
	retval = retval * 2;
	if (sec) *sec = (long)(qpc / freq) + addsec;
	if (usec) *usec = (long)((qpc % freq) * 1000000 / freq);
	#endif
}

/* get clock in millisecond 64 */
static inline IINT64 iclock64(void)
{
	long s, u;
	IINT64 value;
	itimeofday(&s, &u);
	value = ((IINT64)s) * 1000 + (u / 1000);
	return value;
}

static inline IUINT32 iclock()
{
	return (IUINT32)(iclock64() & 0xfffffffful);
}


int output(const char *buf, int len, struct IKCPCB *kcp, void *user)
{
    printf("output!\n");
    if(flag == 0)
        UdpClientSend(sockfd,(char*)buf,len);
    else 
        UdpServerSend(sockfd,(char*)buf,len);
}

int SendData(ikcpcb *kcp, char *buf,int len)
{
    int ret = 0;
    if(kcp == NULL) return -1;
    ret = ikcp_send(kcp,(const char*)buf,len);
    if(ret != 0) return ret;
    
    return 0;
}

int RecvData(ikcpcb *kcp,char *buf)
{
    if(kcp == NULL) return -1;
    char recvBuf[1024];
    if(flag == 0)
        UdpClientRecvfrom(sockfd,recvBuf,sizeof(recvBuf));
    else
        UdpServerRecvfrom(sockfd,recvBuf,sizeof(recvBuf));
    int num = ikcp_input(kcp,(const char*)recvBuf,strlen((const char*)recvBuf));
    int size = ikcp_recv(kcp,buf,num);
    
    return size;
}

void *Thread_Update(void *arg){
    while(1)
    {
        usleep(10);
        ikcp_update(kcp,iclock());
    }
}


int main(int argc,char *argv[])
{
    
    char recvBuf[1024];
    memset(recvBuf,0,1024);
    int flag = atoi(argv[1]);
    conv = 0x11223344;
    kcp = ikcp_create(conv,(void *)flag);
    kcp->output = output;

    pthread_t thread_id;
    int ret = pthread_create(&thread_id,NULL,Thread_Update,NULL);
    if(ret){
        printf("pthread_create faild.");
        return 0;
    }

    if(flag == 0){
        sockfd = InitUdpClient();
        char *msg = "kcp test!";
        printf("send msg!\n");
        if(SendData(kcp,msg,strlen((const char*)msg)) != 0) perror("SendData Error!");
        if(RecvData(kcp,recvBuf) <= 0) perror("RecvData Error!");
        printf("%s\n",recvBuf);

        close(sockfd);
    }
    else
    {
        sockfd = InitUdpServer();
        int ret = 0;
        do{ret = RecvData(kcp,recvBuf);}while (ret <= 0);
        
        if(SendData(kcp,recvBuf,strlen((const char*)recvBuf)) != 0) perror("SendData Error!");

        close(sockfd);
    }
    pthread_join(thread_id,NULL);

    return 0;
}
