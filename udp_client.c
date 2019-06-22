//socket udp 客户端
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <string.h>

struct sockaddr_in addr;

int InitUdpClient()
{
    //创建socket对象
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);

    //创建网络通信对象
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    return sockfd;
}

int UdpClientSend(int sockfd,char *buf,int bufLen)
{
    sendto(sockfd,buf,bufLen,0,(struct sockaddr*)&addr,sizeof(addr));
    return  0;
}

int UdpClientRecvfrom(int sockfd,char *recvBuf,int revcLen)
{
    socklen_t len=sizeof(addr);
    recvfrom(sockfd,recvBuf,revcLen,0,(struct sockaddr*)&addr,&len);
    return 0;
}

#if 0
int main()
{
    //创建socket对象
    int sockfd=InitUdpClient();/*socket(AF_INET,SOCK_DGRAM,0);

    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
*/
    
        char *buf="send msg!";
	char recvBuf[100];
	memset(recvBuf,0,100);
        
	UdpClientSend(sockfd,buf,strlen(buf));
        UdpClientRevcfrom(sockfd,recvBuf,sizeof(recvBuf));

        printf("recv data:%s\n",recvBuf);

    
    close(sockfd);

}
#endif

