#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

extern int InitUdpServer();
extern int UdpServerRecvfrom(int sockfd,char *recvBuf,int recvLen);
extern int UdpServerSend(int sockfd,char *buf,int bufLen);

#endif
