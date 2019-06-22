#ifndef __UDP_CLIENT_H__
#define __UDP_CLIENT_H__


extern int InitUdpClient();
extern int UdpClientRecvfrom(int sockfd,char *recvBuf,int recvLen);
extern int UdpClientSend(int sockfd,char *buf,int bufLen);


#endif