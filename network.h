#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef sockaddr* SP;

class NetWork
{
	int sock;			//	socket描述符
	int type;			//	通信协议类型
	sockaddr_in addr;	//	通信地址
	socklen_t addrlen;	//	地址结构体字节数
	bool is_svr;		//	是否是服务端

public:
	NetWork(void);
	NetWork(int type,const char* ip,uint16_t port,bool is_svr=false);
	~NetWork(void);
	bool open(void);	//	创建socket 绑定 监听 连接
	NetWork* accept(void);	//	等待连接
	int send(const void* buf,size_t len,int flag=0);
	int send(const char* buf,int flag=0);
	int recv(void* buf,size_t len,int flag=0);
};

#endif//NETWORK_H


