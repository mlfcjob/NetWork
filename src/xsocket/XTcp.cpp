
#ifdef _WIN32
#include <Windows.h>
#define socklen_t int
#endif

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#define closesocket close
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "XTcp.h"

#pragma comment(lib, "Ws2_32.lib")

XTcp::XTcp()
{
	static bool isFirst = true;
	if (isFirst) {
#ifdef _WIN32
		isFirst = false;
		WSADATA ws;
		WSAStartup(MAKEWORD(2, 2), &ws);
		isFirst = false;
#endif
	}
}

int XTcp::CreateSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("create socket failed\n");
		return -1;
	}

	return sock;
}

bool XTcp::Bind(unsigned short port)
{
	if (sock <= 0) {
		CreateSocket();
	}

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		printf("bind failed.\n");
		return false;		
	}

	printf("bind port %d success.\n", port);
	listen(sock, 10);
	return true;
}

XTcp XTcp::Accept()
{
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);

	if (client <= 0) {
		printf("accept failed.\n");
		return tcp;
	}
	printf("accept client %d. \n", client);

	char *ip = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ip);
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	printf("client, ip:%s, port:%d.\n", tcp.ip, tcp.port);

	return tcp;
}
void XTcp::Close()
{
	if (sock <=0) return;
	closesocket(sock);
}

int XTcp::Recv(char *buf, int bufsize)
{
	return recv(sock, buf, bufsize, 0);
}

int XTcp::Send(const char *buf, int sendsize)
{
	int sendedsize = 0;

	while (sendedsize != sendsize) {
		int len = send(sock, buf + sendedsize, sendsize - sendedsize, 0);
		if (len <= 0) break;
		sendedsize += len;
	}
	return sendedsize;
}


XTcp::~XTcp()
{
}

bool XTcp::Connect(const char *ip, unsigned short port, int timeoutms)
{
	if (sock <= 0) CreateSocket();

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	SetBlock(false);
	fd_set set;  //文件句柄数组

	if ((connect(sock, (sockaddr *)&saddr, sizeof(saddr))) != 0) {
		FD_ZERO(&set);
		FD_SET(sock, &set);
		timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = timeoutms * 1000;
		if (select(sock + 1, (fd_set*)0, &set, 0, &tm) <= 0) {
			printf("connected timout or error!.\n");
			printf("connect %s:%d failed: %s.\n", ip, port, strerror(errno));
			return false;
		}
	}

	SetBlock(true);
	printf("connect success.\n");
	return true;
}


bool XTcp::SetBlock(bool isblock)
{
	if (sock <= 0) return false;

#ifdef _WIN32
	unsigned long ul = 0;   // 0: block 1:unblock

	if (!isblock) {
		ul = 1;
	}
	ioctlsocket(sock, FIONBIO, &ul);
#else
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0) {
		return false;
	}

	if (isblock) {
		flags = flags & ~O_NONBLOCK;
	}
	else {
		flags = flags | O_NONBLOCK;
	}

	if (fcntl(sock, F_SETFL, flags) != 0) {
		return false;
	}
#endif

	return true;
}