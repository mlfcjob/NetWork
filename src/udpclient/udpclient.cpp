#include <iostream>

#ifdef _WIN32
#include <windows.h>
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
	WSAData ws;
	WSAStartup(MAKEWORD(2,2), &ws);
#endif

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		cout << "create sock failed" << endl;
	}

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int slen = sizeof(saddr);
	char buf[10240] = {0};
	int len = sendto(sock, "12345", 6, 0, (sockaddr*)(&saddr), sizeof(saddr));

	cout << "sendto size is " << len << endl;

	int re = recvfrom(sock, buf, sizeof(buf), 0, 0, 0);
	if (re < 0)
	{
		cout << "recvfrom failed. " << endl;
	}
	cout << "s--->c£º" << buf << endl;
	getchar();
	return 0;
}
