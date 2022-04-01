#pragma once

#include "psInterface.h"

#include <string>
#include <WinSock2.h>

class TcpServer: public PsInterface
{
private:
	bool init();
	bool deinit();

	std::string ipAddr;
	int port;
	SOCKET serverSocket, clientSocket;

public:
	TcpServer(const std::string& ipAddr, int port);

	bool open();
	bool close();

	bool connect();
	bool disconnect();

	size_t write(const uint8_t* data, size_t size);
	size_t read(uint8_t* data, size_t size);

	void error(const std::string& message);
};

