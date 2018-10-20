#include <cstdio>
#include <WinSock2.h>
#include <string>
#include <cstdint>
#include <iostream>
#include <vector>
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	std::string fileList;

	std::string directory = "";

	WIN32_FIND_DATA fileData;
	HANDLE hFind;
	int index = 1;





	uint16_t port = 8888;
	
	//initialize WinSock 2.2
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	//Creat a socket using TCP/IP
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//Fill Server Address info
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(8888);

	printf("Listening on port 8888.....");
	
	//Bind the address to our listener socket and listen for connection
	bind(listener, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));
	listen(listener, 10);
	
	//server forever
	while(true)
	{
		//accept incoming client connection **Blocking**
		SOCKET client = accept(listener, NULL, NULL);
		printf("Conected\n");


		//we do this recieve -> socket -> send -> close
		
		int byteSent = send(client, fileList.c_str(), fileList.length(), 0);

		if (byteSent == SOCKET_ERROR)
		{
			printf("recv failed\n");
			return -1;
		}
		if (byteSent == 0)
		{
			printf("Connection Closed\n");
		}
		else
		{
			
				
			
		}

		
		closesocket(client);
	}

	//WinSock Shutdown
	WSACleanup();

	return 0;
}