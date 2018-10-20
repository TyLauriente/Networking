#define _WINSOCK_DEPRICATED_NO_WARNINGS

#include <string>
#include <cstdio>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	printf("hello world!!!");

	const char* hostAddress = "127.0.0.1";
	uint16_t port = 8888;

	//initialize WinSock 2.2
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//Creat a socket using TCP/IP
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Resolve Host using address
	in_addr iaHost;
	iaHost.s_addr = inet_addr(hostAddress);


	HOSTENT* hostEntry = gethostbyaddr((const char*)&iaHost, sizeof(struct in_addr), AF_INET);

	//Fill Server Address info
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	serverInfo.sin_port = htons(8888);
	int res = connect(mySocket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));


	printf("Connected to server");

	std::string itemList;

	//Wait for server responce **Bocking**
	char buffer[1024];
	int byteRecieved = recv(mySocket, buffer, std::size(buffer) - 1, 0);

	if (byteRecieved == SOCKET_ERROR)
	{
		printf("recv failed\n");
		system("pause");
		return -1;
	}
	if (byteRecieved == 0)
	{
		printf("Connection Closed\n");
	}
	else
	{
		buffer[byteRecieved] = '\0';
		printf("Recieved: %s\n", buffer);
	}

	//close all socket
	closesocket(mySocket);

	//WinSock Shutdown
	WSACleanup();

	system("pause");
	return 0;
}