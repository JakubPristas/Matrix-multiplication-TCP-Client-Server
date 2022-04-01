#pragma once
#include "tcp.h"

void drawMatrix(int** board, int aR, int aC);
void numEnter(int** board, int aR, int aC);
int** mulMat(int** aMat1, int** aMat2, int aR1, int aC1, int aR2, int aC2);
int** elementWrite(TcpServer tcpServer, int** board, int aR, int aC);

