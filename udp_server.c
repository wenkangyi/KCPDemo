//socket udp 服务端
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <string.h>

struct sockaddr_in cli;
    

int InitUdpServer()
{
    //创建socket对象
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);

    //创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //绑定socket对象与通信链接
    int ret =bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(0>ret)
    {
        printf("bind\n");
        return -1;
    }
    return sockfd;
}

int UdpServerRecvfrom(int sockfd,char *recvBuf,int recvLen)
{
    socklen_t len=sizeof(cli);
    int num = recvfrom(sockfd,recvBuf,recvLen,0,(struct sockaddr*)&cli,&len);
    return num;
}

int UdpServerSend(int sockfd,char *buf,int bufLen)
{
    socklen_t len=sizeof(cli);
    sendto(sockfd,buf,bufLen,0,(struct sockaddr*)&cli,len);
}

#if 0
int main()
{
    //创建socket对象
    int sockfd=InitUdpServer();/*socket(AF_INET,SOCK_DGRAM,0);*/
    
    /*//创建网络通信对象
    struct sockaddr_in addr;
    addr.sin_family =AF_INET;
    addr.sin_port =htons(1324);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //绑定socket对象与通信链接
    int ret =bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    if(0>ret)
    {
        printf("bind\n");
        return -1;
    }*/

    

    char buf[100];
    memset(buf,0,sizeof(buf));
    //recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,&len);
    UdpServerRecvfrom(sockfd,buf,sizeof(buf));
    printf("recv msg =%s\n",buf);

    //sendto(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&cli,len);
    int len = strlen(buf);
    printf("buf len=%d\n",len);
    UdpServerSend(sockfd,buf,strlen(buf));
    
    close(sockfd);

}

#endif
