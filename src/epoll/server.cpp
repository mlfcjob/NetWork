#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>

#include <sys/epoll.h>

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

	// create epoll
	int epfd = epoll_create(256);

	// register event
	epoll_event ev;
	ev.data.fd = server.sock;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server.sock, &ev);

	epoll_event events[20];

	char buf[1024] = {0};
	const char *msg = "HTTP/1.1 200 OK\r\nContent-Length:1\r\n\r\nX";
	int size = strlen(msg);

	server.SetBlock(false);

	for (;;) {
		int count = epoll_wait(epfd, events, 20, 500);
		if (count <= 0) continue;

		for (int i = 0; i < count; i++)
		{
			if (events[i].data.fd == server.sock) {
				
				for (;;) {
					XTcp client = server.Accept();
					if (client.sock <= 0) break;
					ev.data.fd = client.sock;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
				}
			}
			else {
				XTcp client;
				client.sock = events[i].data.fd;
				client.Recv(buf, 1024);
				client.Send(msg,size);
				epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);
				client.close();
			}
		}
	}
	server.Close();

	return 0;
}
