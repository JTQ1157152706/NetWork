#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "network.h"

using namespace std;

NetWork::NetWork(void)
{
	type = SOCK_STREAM;
	is_svr = false;
}

NetWork::NetWork(int type,const char* ip,uint16_t port,bool is_svr):type(type),is_svr(is_svr)
{
	//	不建议在构造函数创建socket等操作，因为可能会失败，而构造函数没有返回值，所以无法得知是否失败，就无法停止操作
	addrlen = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
}

NetWork::~NetWork(void)
{
	close(sock);	
}

//	创建socket 绑定 监听 连接
bool NetWork::open(void)
{
	sock = socket(AF_INET,type,0);
	if(0 > sock)
	{
		perror("NetWork::open");
		return false;
	}

	if(is_svr)
	{
		if(bind(sock,(SP)&addr,addrlen))
		{
			perror("bind");
			return false;
		}
		if(SOCK_STREAM == type)
		{
			if(listen(sock,20))
			{
				perror("listen");
				return false;
			}
		}
	}
	else if(SOCK_STREAM == type)
	{
		if(connect(sock,(SP)&addr,addrlen))
		{
			perror("connect");
			return false;
		}
	}
	return true;
}

//	等待连接
NetWork* NetWork::accept(void)
{
	if(SOCK_STREAM != type || !is_svr)
	{
		cout << "只能是TCP服务端执行此函数" << endl;
		return NULL;
	}

	NetWork* nw = new NetWork;
	nw->sock = ::accept(sock,(SP)&addr,&addrlen);
	if(0 > nw->sock)
	{
		perror("NetWork::accept");
		delete nw;
		return NULL;
	}
	return nw;
}

int NetWork::send(const void* buf,size_t len,int flag)
{
	if(SOCK_STREAM == type)
	{
		return ::send(sock,buf,len,flag);	
	}
	else
	{
		return sendto(sock,buf,len,flag,(SP)&addr,addrlen);	
	}
}

int NetWork::send(const char* buf,int flag)
{
	if(SOCK_STREAM == type)
	{
		return ::send(sock,buf,strlen(buf)+1,flag);	
	}
	else
	{
		return sendto(sock,buf,strlen(buf)+1,flag,(SP)&addr,addrlen);	
	}
}

int NetWork::recv(void* buf,size_t len,int flag)
{
	if(SOCK_STREAM == type)
	{
		return ::recv(sock,buf,len,flag);	
	}
	else
	{
		return recvfrom(sock,buf,len,flag,(SP)&addr,&addrlen);	
	}
}
