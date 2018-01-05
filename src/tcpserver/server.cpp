#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>

class TcpThread {
public:
	void Main() {
		char buf[1024] = {0};
		for (;;)
		{
			int recvlen = client.Recv(buf, sizeof(buf) - 1);
			buf[recvlen] = '\0';

			if (strstr(buf, "quit") != NULL)
			{
				const char *re = "quit success\n";
				client.Send(re, strlen(re) + 1);
				break;
			}
			printf("%s\n", buf);
			client.Send("ok\n", 4);
		}
		client.Close();
		delete this;
	}

	XTcp client;
};

int main(int argc, char *argv[])
{
	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}
	
	XTcp server;
	server.Bind(port);

	for (;;) {
		XTcp client = server.Accept();
		TcpThread *th = new TcpThread();
		th->client = client;

		std::thread sth(&TcpThread::Main, th);
		sth.detach();
	}
	server.Close();

	return 0;
}
