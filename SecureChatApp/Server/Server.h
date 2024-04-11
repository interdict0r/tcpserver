#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class CServer {
public:
	CServer(const std::string& ip, int port);
	~CServer();

	void Start();
	void Stop();

private:
	std::string ip_address;
	int port;
	SOCKET server_socket;
	bool is_running;


	bool initializeSocket();
	void closeSocket();
};