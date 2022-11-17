#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "WS2_32.lib")


using namespace std;

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET SendSocket = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN SendSockAddr;
	ZeroMemory(&SendSockAddr, sizeof(SendSockAddr));
	SendSockAddr.sin_family = PF_INET;
	SendSockAddr.sin_port = htons(4949);
	SendSockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char Buffer[1024] = { 0, };
	strcpy(Buffer, "Get File");

	int SentBytes = sendto(SendSocket, Buffer, (int)strlen(Buffer) + 1, 0, (SOCKADDR*)&SendSockAddr, sizeof(SendSockAddr));


	int RecvBytes = 0;
	FILE* OutputFile = fopen("수능대박2.jpg", "wb");
	do
	{
		int SendSockAddrLength = sizeof(SendSockAddr);
		RecvBytes = recvfrom(SendSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&SendSockAddr, &SendSockAddrLength);

		size_t ReadSize = fwrite(Buffer, 1, RecvBytes, OutputFile);

	} while ((strcmp(Buffer, "EndFile") != 0));

	fclose(OutputFile);

	closesocket(SendSocket);



	WSACleanup();
}