
#include "XTcp.h"

int main(int argc, char *argv[])
{
	XTcp client;

	client.Connect("192.168.103.137", 8080);
	client.Send("Client", 6);

	char buf[1024] = {0};
	client.Recv(buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}