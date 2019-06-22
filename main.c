#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ikcp.h"

#ifdef __unix
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#endif

IUINT32 conv = 0;
void *user = NULL;
ikcpcb *kcp = NULL;

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


int output(const char *buf, int len, struct IKCPCB *kcp, void *user)
{
    printf("output!\n");
}

int SendData(ikcpcb *kcp, char *buf,int len)
{
    int ret = 0;
    if(kcp == NULL) return -1;
    ret = ikcp_send(kcp,buf,len);
    if(ret != 0) return ret;
    ikcp_update(kcp,iclock());
    return 0;
}

int RecvData(ikcpcb *kcp,char *buf)
{
    if(kcp == NULL) return -1;
    int size = ikcp_recv(kcp,buf,1024);
    
    return size;
}

int main(int argc,char *argv[])
{
    
    char *revBuf = NULL;

    int num = atoi(argv[1]);
    kcp = ikcp_create(conv,(void *)num);
    kcp->output = output;

    if(num == 0){
        const char *msg = "kcp test!";
        printf("send msg!\n");
        if(SendData(kcp,msg,strlen(msg)) != -1) perror("SendData Error!");
        if(RecvData(kcp,revBuf) <= 0) perror("RecvData Error!");
        if(NULL != revBuf)
        {
            printf("%s\n",revBuf);
            free(revBuf);
        }
    }
    else
    {
        if(RecvData(kcp,revBuf) <= 0) perror("RecvData Error!");
        if(SendData(kcp,revBuf,strlen(revBuf)) != -1) perror("SendData Error!");
        free(revBuf);
    }
    

    return 0;
}
