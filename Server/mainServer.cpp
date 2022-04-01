#include <iostream>
#include <string>
#include <cstdlib>
#include "tcp.h"
#include "Matrix.h"

int main(int argc, char* argv[])
{
	std::string ipAddr;
	std::cout << "Enter ip address: ";
	std::cin >> ipAddr;

	int port;
	std::cout << "Enter port: ";
	std::cin >> port;

	TcpServer tcpServer(ipAddr, port);
	if (!tcpServer.open())
	{
		return -1;
	}

	std::cout << "Ready!" << std::endl;

	if (!tcpServer.connect())
	{
		return -1;
	}

	std::cout << "Client connected!" << std::endl;

	char parameters[256];
	std::string message;
	int R1, C1, R2, C2;

	//na��tanie rozmerov mat�c prostredn�ctvom komunik�cie s klientom a zistenie �i sa matica m��e n�sobi�
	//pod�a vyhodnotenia za�le klientovy 0 alebo 1 
	while (true) 
	{
		memset(parameters, 0, 256);
		tcpServer.read((uint8_t*)parameters, 256);

		R1 = (int)parameters[0] - 48;
		C1 = (int)parameters[1] - 48;
		R2 = (int)parameters[2] - 48;
		C2 = (int)parameters[3] - 48;

		if (C1 != R2)
		{
			message = '0';
			tcpServer.write((const uint8_t*)message.c_str(), message.length() + 1);
		}
		else 
		{
			message = '1';
			tcpServer.write((const uint8_t*)message.c_str(), message.length() + 1);
			break;
		}
	}

	//deklarovanie mat�c 
	int** mat1 = new int* [R1];
	for (int i = 0; i < R1; i++)
	{
		mat1[i] = new int[C1];
	}

	int** mat2 = new int* [R2];
	for (int i = 0; i < R2; i++)
	{
		mat2[i] = new int[C2];
	}
	//na�itanie jednotlivych prvkov mat�c prostredn�ctvom komunik�cie s klientom
	mat1 = elementWrite(tcpServer, mat1, R1, C1);
	mat2 = elementWrite(tcpServer, mat2, R2, C2);

	//klientovy odo�le znak "x", ��m mu d� vedie�, �e v�etky prvky mat�c s� na��tane
		message = "x";
		tcpServer.write((const uint8_t*)message.c_str(), message.length() + 1);

	//vyn�sobenie mat�c a n�sledne zasielanie po jednotliv�ch prvkoch klientovy
		message = "";
		for (int i = 0; i < R1; i++) {
			for (int j = 0; j < C2; j++) {
				message = std::to_string(mulMat(mat1, mat2, R1, C1, R2, C2)[i][j]);
				if (tcpServer.write((const uint8_t*)message.c_str(), message.length() + 1) == message.length() + 1)
				{
					std::cout << "Sent: " << message << std::endl;
				}
			}
		}
	
	tcpServer.disconnect();

	if (!tcpServer.close())
	{
		return -1;
	}

	return 0;
}