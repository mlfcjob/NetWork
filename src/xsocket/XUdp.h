#ifndef UDP_H
#define UDP_H

#ifdef _WIN32
#ifdef XSOCKET_EXPORTS
#define XSOCKET_API __declspec(dllexport)
#else
#define XSOCKET_API __declspec(dllimport)
#endif
#else
#define XSOCKET_API 
#endif



#pragma once
class XSOCKET_API XUdp
{
public:
	XUdp();
	~XUdp();

	int CreateSocket();
	bool Bind(unsigned short port);
	void Close();
	int Recv(char *buf, int bufsize);

	char  ip[16];
	unsigned short port = 0;
protected:
	int sock = 0;
private:
	void *addr = 0;
};

#endif

