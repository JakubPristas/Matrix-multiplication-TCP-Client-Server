#include <iostream>
#include <string>
#include <stdlib.h>
#include "tcp.h"

int main(int argc, char* argv[])
{
	std::string ipAddr;
	std::cout << "Enter ip address: ";
	std::cin >> ipAddr;
	
	int port;
	std::cout << "Enter port: ";
	std::cin >> port;
	
	TcpClient tcpClient(ipAddr, port);
	if (!tcpClient.open())
	{
		return -1;
	}

	std::cout << "Ready!" << std::endl;
	system("CLS");
	char R1, C1, R2, C2;
	std::string parameters;
	char message[256];
	//zadanie rozmerov matíc a komunikácia so serverom èi je možné matice násobi
	while (true)
	{
		//first matrix
		std::cout << "Enter number of row of the first matrix: " << std::endl;
		std::cin >> R1;

		std::cout << "Enter number of col of the first matrix: " << std::endl;
		std::cin >> C1;

		//second matrix
		std::cout << "Enter number of row of the second matrix: " << std::endl;
		std::cin >> R2;

		std::cout << "Enter number of col of the second matrix: " << std::endl;
		std::cin >> C2;

		parameters = "";
		parameters += R1;
		parameters += C1;
		parameters += R2;
		parameters += C2;

		tcpClient.write((const uint8_t*)parameters.c_str(), parameters.length() + 1);
		
		memset(message, 0, 256);
		tcpClient.read((uint8_t*)message, 256);
		
		int isRight = (int)message[0] - 48;
		if(isRight == 0)
		{
			system("CLS");
			std::cout << "This matrix cant be multiply! Try again." << std::endl;
		}
		else 
		{	
			system("CLS");
			break;
		}
		
	}
	//zapisovanie jednotlivých prvkov matice
	// pri naèítaní znaku "x", už len prijma výsledok
	
	int i = 0;
	int j = (int)R1 - 48;
	int k = 1;
	while (true)
	{
		char chNum[256];
		memset(chNum, 0, 256);
		if (tcpClient.read((uint8_t*)chNum, 256) > 0)
		{
			if (chNum[0] == 'x') 
			{
				while (true)
				{
					i++;
					if (tcpClient.read((uint8_t*)message, 256) > 0)
					{
						if (i == j*k) 
						{
							std::cout << message << std::endl;
							k++;
						}
						else 
						{
							std::cout << message << " ";
						}
					}
				}
			}
			else
			{
				int iNum = (int)chNum[0];
				std::cout << "Enter a number of " << iNum << ".element." << std::endl;
			}
		}
		
		std::string element;
		std::cin >> element;

		tcpClient.write((const uint8_t*)element.c_str(), element.length() + 1);
	}

	if (!tcpClient.close())
	{
		return -1;
	}

	return 0;
}