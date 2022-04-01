#include "tcp.h"
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")

bool TcpClient::init()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		error("Error in WSA startup");
		return false;
	}

	this->clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->clientSocket == INVALID_SOCKET)
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

	if (connect(this->clientSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR) 
	{
		error("Error connecting to client");
		deinit();
		return false;
	}

	return true;
}

bool TcpClient::deinit()
{
	bool ret = closesocket(this->clientSocket) != SOCKET_ERROR;
	WSACleanup();
	return ret;
}

TcpClient::TcpClient(const std::string& ipAddr, int port)
{
	this->ipAddr = ipAddr;
	this->port = port;
	this->clientSocket = INVALID_SOCKET;
}

bool TcpClient::open()
{
	if (this->clientSocket != INVALID_SOCKET)
	{
		return true;
	}

	return init();
}

bool TcpClient::close()
{
	if (this->clientSocket == INVALID_SOCKET)
	{
		return true;
	}

	return deinit();
}

size_t TcpClient::write(const uint8_t* data, size_t size)
{
	int bytesWritten = send(this->clientSocket, (const char*)data, size, 0);
	if (bytesWritten == SOCKET_ERROR) 
	{
		error("Cannot write data");
		return -1;
	} 

	return bytesWritten;
}

size_t TcpClient::read(uint8_t* data, size_t size)
{
	int bytesRead = recv(this->clientSocket, (char*)data, size, 0);
	if (bytesRead == SOCKET_ERROR) 
	{
		error("Cannot read data");
		return -1;
	}
	
	return bytesRead;
}

void TcpClient::error(const std::string& message)
{
	std::cerr << message << ", code: " << GetLastError() << std::endl << std::flush;
}
