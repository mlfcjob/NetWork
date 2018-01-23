tcpserver:server.cpp
	g++ $+ -o $@ -L../xsocket -lxsocket -I../xsocket -std=c++11 -lpthread
	cp $@ /usr/bin
clean:
	rm -rf tcpserver
