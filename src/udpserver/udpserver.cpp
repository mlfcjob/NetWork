#include <iostream>

#ifdef _WIN32
#include <windows.h>
#define socklen_t int
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

using namespace std;



int main(int argc, char *argv[])
{
	unsigned short port = 8080;

	if (argc > 1)
	{
		port = atoi(argv[1]);
	}

#ifdef _WIN32
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock <= 0)
	{
		cout << "create socket failed! " << endl;
		return -1;
	}

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);

	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		cout << "bind port" << port << " failed." << endl;
	}

	cout << "bind port" << port << " success." << endl;

	listen(sock, 10);

	sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	char buf[10240] = { 0 };
	int re = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)(&client_addr), &len);
	if (re <= 0)
	{
		cout << "Recvfrom fialed!" << endl;
		return -3;
	}

	if (re > 0)
	{	
		buf[re] = '\0';
		cout << buf << endl;
	}


	cout <<"c--->s:" << inet_ntoa(client_addr.sin_addr)  <<": " << ntohs(client_addr.sin_port) << endl;
	sendto(sock, "67890", 5, 0, (sockaddr*)(&client_addr), sizeof(client_addr));

	getchar();
	return 0;
}



