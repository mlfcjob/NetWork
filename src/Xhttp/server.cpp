
#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <regex>


using namespace std;



class HttpThread {
public:
	void Main() {
		char buf[10000] = { 0 };

		for (;;) {
			int recvlen = client.Recv(buf, sizeof(buf) - 1);
			if (recvlen <= 0) {
				Close();
				return;
			}

			buf[recvlen] = '\0';
			printf("==========recv======\n%s=================\n", buf);


			//GET / HTTP/1.1
			//Host: 192.168.103.137:81
			//Connection: keep-alive
			//Cache-Control: max-age=0
			//Upgrade-Insecure-Requests: 1
			//User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.96 Safari/537.36
			//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
			//Accept-Encoding: gzip, deflate, sdch
			//Accept-Language: zh-CN,zh;q=0.8

			string src = buf;
			string pattern = "^([A-Z]+) (.+) HTTP/1";
			regex r(pattern);
			smatch mas;
			regex_search(src, mas, r);
			if (mas.size() == 0) {
				printf("%s failed!\n", pattern.c_str());
				Close();
				return;
			}


			string type = mas[1];
			string path = mas[2];
			printf("type:%s.\n", type.c_str());
			printf("path:%s.\n", path.c_str());
			if (type != "GET") {
				Close();
				return;
			}

			string filename = path;
			if (path == "/")
			{
				filename = "/index.html";
			}

			string filepath = "www";
			filepath += filename;
			FILE *fp = fopen(filepath.c_str(), "rb");
			if (fp == NULL)
			{
				printf("open file: %s failed, %s.\n", filepath.c_str(), strerror(errno));
				Close();
				return;
			}

			fseek(fp, 0, SEEK_END);
			int filesize = ftell(fp);
			fseek(fp, 0, 0);
			printf("file size is %d\n", filesize);

			//回应http GET请求
			// msg header
			string rmsg = "";
			rmsg = "HTTP/1.1 200 OK\r\n";
			rmsg += "Server: XHttp\r\n";
			rmsg += "Content-Type: text/html\r\n";
			rmsg += "Content-Length: ";
			char bsize[128] = { 0 };
			sprintf(bsize, "%d", filesize);
			rmsg += bsize;
			rmsg += "\r\n\r\n";

			// send msg header
			int sendSize = client.Send(rmsg.c_str(), rmsg.size());
			printf("sendsize = %d \n", sendSize);
			printf("====send======= \n%s\n===========", rmsg.c_str());

			// send content
			for (;;) {
				int len = fread(buf, 1, sizeof(buf), fp);
				if (len <= 0) break;
				int re = client.Send(buf, len);
				if (re <= 0) break;
			}
		}
		Close();
	}

	void Close()
	{
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
		HttpThread *th = new HttpThread();
		th->client = client;

		std::thread sth(&HttpThread::Main, th);
		sth.detach();
	}
	server.Close();

	return 0;
}
