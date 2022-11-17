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
	bind(SendSocket, (SOCKADDR*)&SendSockAddr, sizeof(SendSockAddr));

	char Buffer[1024] = { 0, };

	SOCKADDR_IN RecvSockAddr;
	ZeroMemory(&RecvSockAddr, sizeof(RecvSockAddr));
	int RecvScokAddrLength = sizeof(RecvSockAddr);
	//파일 요청, blocking mode
	int RecvBytes = recvfrom(SendSocket, Buffer, sizeof(Buffer), 0, (SOCKADDR*)&RecvSockAddr, &RecvScokAddrLength);
	if (strcmp(Buffer, "Get File") == 0) //클라이언트에서 보내 준 정보
	{
		FILE* InputFile = fopen("수능대박.jpg", "rb");
		do
		{
			size_t ReadSize = fread(Buffer, 1, 1024, InputFile);

			int SentBytes = sendto(SendSocket, Buffer, (int)ReadSize, 0, (SOCKADDR*)&RecvSockAddr, sizeof(RecvSockAddr));
		} while (!feof(InputFile));

		fclose(InputFile);

		//파일 끝
		strcpy(Buffer, "EndFile");
		sendto(SendSocket, Buffer, (int)strlen(Buffer) + 1, 0, (SOCKADDR*)&RecvSockAddr, sizeof(RecvSockAddr));
	}

	closesocket(SendSocket);

	WSACleanup();
}