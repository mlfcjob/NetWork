#include <iostream>
#include "XUdp.h"
#include <string>
#include <regex>

using namespace std;



int main(int argc, char *argv[])
{
	unsigned short port = 514;

	XUdp syslog;
	if (!syslog.Bind(port))
	{
		return -1;
	}

	char buf[2000] = {0};
	for(;;)
	{
		int len = syslog.Recv(buf, sizeof(buf));
		if (len <= 0) {
			continue;
		}

		buf[len] = '\0';
		//cout << buf << endl;

		string src = buf;
		string p = "Failed password for ([a-zA-Z0-9]+) from ([0-9.]+)";

		regex r(p);
		smatch mas;
		regex_search(src, mas, r);

		//cout << "mas:[0]" << mas[0] << ";[1] " << mas[1] << ";[2]" << mas[2] << endl;
		if (mas.size() > 0)
		{
			cout << "Warning: User " << mas[1] << "@" << mas[2] << "login failed"<<endl;
		}

	}

	return 0;
}
