#include "Server.h"
#include <iostream>

CServer::CServer(const std::string& ip, int port) 
	: ip_address(ip), port(port), server_socket(INVALID_SOCKET), is_running(false) {}

CServer::~CServer() {}

void CServer::Start() {
	if(!initializeSocket()) {
		return;
	}

	is_running = true;
	while(is_running) {
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);
		SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_size);
		if(client_socket == INVALID_SOCKET) {
			std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
			break;
		}

		char buffer[4096];
		int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
		if(bytes_received == SOCKET_ERROR) {
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			closesocket(client_socket);
			break;
		}

		std::string message(buffer, bytes_received);
		std::cout << "Received: " << message << std::endl;

		std::string response = "Hello from server!";
		int bytes_sent = send(client_socket, response.c_str(), response.size() + 1, 0);
		if(bytes_sent == SOCKET_ERROR) {
			std::cerr << "send failed: " << WSAGetLastError() << std::endl;
			closesocket(client_socket);
			break;
		}

		closesocket(client_socket);
	}

	closeSocket();
}


bool CServer::initializeSocket() {
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if(result != 0) {
		std::cerr << "WSAStartup failed: " << result << std::endl;
		return false;
	}

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_socket == INVALID_SOCKET) {
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip_address.c_str(), &addr.sin_addr);

	result = bind(server_socket, (sockaddr*)&addr, sizeof(addr));
	if(result == SOCKET_ERROR) {
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return false;
	}

	result = listen(server_socket, SOMAXCONN);
	if(result == SOCKET_ERROR) {
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		closesocket(server_socket);
		WSACleanup();
		return false;
	}


	// print server startup was successful
	std::cout << "Server startup was successful" << std::endl;

	return true;
}

void CServer::closeSocket() {
	closesocket(server_socket);
	WSACleanup();
}