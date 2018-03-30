//#include "stdafx.h"
#include "XUdp.h"

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

#pragma comment(lib, "Ws2_32.lib")


XUdp::XUdp()
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

	memset(ip, sizeof(ip), 0);

}

int XUdp::CreateSocket()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		printf("create socket failed\n");
		return -1;
	}

	return sock;
}

bool XUdp::Bind(unsigned short port)
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

int XUdp::Recv(char *buf, int bufsize)
{
	if (addr == 0)
	{
		addr = new sockaddr_in();
	}

	if (sock <= 0)
		return 0;
	socklen_t len = sizeof(sockaddr_in);
	
	int re = recvfrom(sock, buf, bufsize, 0, (sockaddr*)addr, &len);
	return re;
}


void XUdp::Close()
{
	if (sock <= 0) return;
	closesocket(sock);
	
	if (addr) {
		delete addr;
		addr = NULL;
	}

	sock = 0;
}



XUdp::~XUdp()
{
}
