#include "tcp.h"
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")

bool TcpServer::init()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) 
	{
		error("Error in WSA startup");
		return false;
	}

	this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->serverSocket == INVALID_SOCKET) 
	{
		error("Error creating socket");
		WSACleanup();
		return false;
	}

	sockaddr_in socketAddr;
	memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr(this->ipAddr.c_str());
	socketAddr.sin_port = htons(this->port);

	if (bind(this->serverSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR)
	{
		error("Error binding socket");
		deinit();
		return false;
	}

	if (listen(this->serverSocket, SOMAXCONN) == SOCKET_ERROR) 
	{
		error("Error listening on socket");
		deinit();
		return false;
	}

	return true;
}

bool TcpServer::deinit()
{
	bool ret = closesocket(this->serverSocket) != SOCKET_ERROR;
	WSACleanup();
	return ret;
}

TcpServer::TcpServer(const std::string& ipAddr, int port)
{
	this->ipAddr = ipAddr;
	this->port = port;
	this->serverSocket = INVALID_SOCKET;
	this->clientSocket = INVALID_SOCKET;
}

bool TcpServer::open()
{
	if (this->serverSocket != INVALID_SOCKET)
	{
		return true;
	}

	return init();
}

bool TcpServer::close()
{
	if (this->serverSocket == INVALID_SOCKET)
	{
		return true;
	}

	return deinit();
}

bool TcpServer::connect()
{
	if (this->clientSocket != INVALID_SOCKET)
	{
		return false;
	}
	//možnos zisti clientovu ip

	this->clientSocket = accept(this->serverSocket, NULL, NULL);
	if (this->clientSocket == INVALID_SOCKET) 
	{
		error("Error accepting client");
		deinit();
		return false;
	}

	return true;
}

bool TcpServer::disconnect()
{
	if (this->clientSocket == INVALID_SOCKET)
	{
		return false;
	}

	return closesocket(this->clientSocket) != SOCKET_ERROR;
}

size_t TcpServer::write(const uint8_t* data, size_t size)
{
	int bytesWritten = send(this->clientSocket, (const char*)data, size, 0);
	if (bytesWritten == SOCKET_ERROR) 
	{
		error("Cannot write data");
		return -1;
	} 

	return bytesWritten;
}

size_t TcpServer::read(uint8_t* data, size_t size)
{
	int bytesRead = recv(this->clientSocket, (char*)data, size, 0);
	if (bytesRead == SOCKET_ERROR) 
	{
		error("Cannot read data");
		return -1;
	}
	
	return bytesRead;
}

void TcpServer::error(const std::string& message)
{
	std::cerr << message << ", code: " << GetLastError() << std::endl << std::flush;
}

