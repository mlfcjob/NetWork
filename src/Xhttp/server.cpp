
#include "XTcp.h"
#include <thread>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <regex>


using namespace std;



//class HttpThread {
//public:
//	void Main() {
//		char buf[10000] = { 0 };
//
//		for (;;) {
//			int recvlen = client.Recv(buf, sizeof(buf) - 1);
//			if (recvlen <= 0) {
//				Close();
//				return;
//			}
//
//			buf[recvlen] = '\0';
//			printf("==========recv======\n%s=================\n", buf);
//
//
//			//GET /index.php?id=1&name=mlf HTTP/1.1
//			//Host: 192.168.103.137:81
//			//Connection: keep-alive
//			//Cache-Control: max-age=0
//			//Upgrade-Insecure-Requests: 1
//			//User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.96 Safari/537.36
//			//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//			//Accept-Encoding: gzip, deflate, sdch
//			//Accept-Language: zh-CN,zh;q=0.8
//
//			string src = buf;
//
//			// /  /index.html /ff/index.php
//
//
//			//string pattern = "^([A-Z]+) /([a-zA-Z0-9]*([.].*)?) HTTP/1";
//			string pattern = "^([A-Z]+) /([a-zA-Z0-9]*)([.][a-zA-Z]*)?)?(.*) HTTP/1";
//			regex r(pattern);
//			smatch mas;
//			regex_search(src, mas, r);
//			if (mas.size() == 0) {
//				printf("%s failed!\n", pattern.c_str());
//				Close();
//				return;
//			}
//
//
//			string type = mas[1];
//			string path = "/";
//			path += mas[2];
//			string filetype = mas[3];
//			string query = mas[4];
//			if (filetype.size() > 0)
//				filetype = filetype.substr(1, filetype.size() - 1);
//
//			printf("type:[%s], path:[%s], fileytpe:[%s], query:[] \n", type.c_str(), path.c_str(), filetype.c_str());
//			if (type != "GET") {
//				printf("Not Get!!!!!.\n");
//				Close();
//				return;
//			}
//
//			string filename = path;
//			if (path == "/")
//			{
//				filename = "/index.html";
//			}
//
//			string filepath = "www";
//			filepath += filename;
//
//			// php-cgi www/index.php > www/index.php.html
//			// php-cgi www/index.php id=1 name=mlf > www/index.php.html
//			if (filetype == "php")
//			{
//				string cmd = "php-cgi ";
//				cmd += filepath;
//				cmd += " ";
//
//
//				// query: id=1&name=mlf ===> id=1 name=mlf
//				for (int i = 0; i < query.size(); i++)
//				{
//					if (query[i] == '&') query[i] = ' ';
//				}
//
//				cmd += query;
//
//				cmd += " > ";
//				filepath += ".html";
//				cmd += filepath;
//
//				printf("%s\n", cmd.c_str());
//				system(cmd.c_str());
//
//			}
//
//
//			FILE *fp = fopen(filepath.c_str(), "rb");
//			if (fp == NULL)
//			{
//				printf("open file: %s failed, %s.\n", filepath.c_str(), strerror(errno));
//				Close();
//				return;
//			}
//
//			fseek(fp, 0, SEEK_END);
//			int filesize = ftell(fp);
//			fseek(fp, 0, 0);
//			printf("file size is %d\n", filesize);
//
//			if (filetype == "php") 
//			{
//				char c = 0;
//				int headsize = 0;
//				while (fread(&c, 1, 1, fp) > 0) {
//					if (c == '\r')
//					{
//						fseek(fp, 3, SEEK_CUR);
//						headsize += 3;
//						break;
//					}
//				}
//
//				filesize = filesize - headsize;
//			}
//
//
//			//回应http GET请求
//			// msg header
//			string rmsg = "";
//			rmsg = "HTTP/1.1 200 OK\r\n";
//			rmsg += "Server: XHttp\r\n";
//			rmsg += "Content-Type: text/html\r\n";
//			rmsg += "Content-Length: ";
//			char bsize[128] = { 0 };
//			sprintf(bsize, "%d", filesize);
//			rmsg += bsize;
//			rmsg += "\r\n\r\n";
//
//			// send msg header
//			int sendSize = client.Send(rmsg.c_str(), rmsg.size());
//			printf("sendsize = %d \n", sendSize);
//			printf("====send======= \n%s\n===========", rmsg.c_str());
//
//			// send content
//			for (;;) {
//				int len = fread(buf, 1, sizeof(buf), fp);
//				if (len <= 0) break;
//				int re = client.Send(buf, len);
//				if (re <= 0) break;
//			}
//		}
//		Close();
//	}
//
//	void Close()
//	{
//		client.Close();
//		delete this;
//	}
//
//	XTcp client;
//};

#include "XHttpserver.h"
#include <signal.h>

int main(int argc, char *argv[])
{
	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}

	XHttpserver server;
	server.Start(port);
	getchar();

	/*XTcp server;
	server.Bind(port);

	for (;;) {
		XTcp client = server.Accept();
		HttpThread *th = new HttpThread();
		th->client = client;

		std::thread sth(&HttpThread::Main, th);
		sth.detach();
	}
	server.Close();*/

	return 0;
}
