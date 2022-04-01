#include <iostream>
#include "matrix.h"
#include "tcp.h"


void drawMatrix(int** board, int aR, int aC)
{
    for (int i = 0; i < aR; i++) {
        for (int j = 0; j < aC; j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
}

void numEnter(int** board, int aR, int aC)
{
    int element = 0;
    for (int i = 0; i < aR; i++) {
        for (int j = 0; j < aC; j++) {
            int num;
            element++;
            std::cout << "Enter a number of " << element << ". element." << std::endl;
            std::cin >> num;
            board[i][j] = num;
        }
    }
}

int** mulMat(int** aMat1, int** aMat2, int aR1, int aC1, int aR2, int aC2) {
    int** rslt = new int* [aR1];
    for (int i = 0; i < aR1; i++)
    {
        rslt[i] = new int[aC2];
    }
    for (int i = 0; i < aR1; i++) {
        for (int j = 0; j < aC2; j++) {
            rslt[i][j] = 0;

            for (int k = 0; k < aR2; k++) {
                rslt[i][j] += aMat1[i][k] * aMat2[k][j];
            }
        }
    }
    return rslt;
}

int** elementWrite(TcpServer tcpServer, int** board, int aR, int aC)
{
    int iNum;
    char chNum[256];
    int element = 0;
    std::string message;
    for (int i = 0; i < aR; i++) {
        for (int j = 0; j < aC; j++) {

            element++;
            message = int(element);
            tcpServer.write((const uint8_t*)message.c_str(), message.length() + 1);

            memset(chNum, 0, 256);
            tcpServer.read((uint8_t*)chNum, 256);

            iNum = atoi(chNum);
            board[i][j] = iNum;
        }
    }
    return board;
}
