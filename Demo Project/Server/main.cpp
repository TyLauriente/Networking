#include <cstdio>
#include <WinSock2.h>
#include <string>
#include <cstdint>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")



std::string GetTextFromBuffer(char buffer[], int size)
{
	std::string text = "    ";

	int index = 0;

	while (buffer[index] != '\0')
	{
		text[index] += buffer[index];
		index++;
	}

	return text;
}


int main(int argc, char* argv[])
{
	const std::string Password = "1234";

	printf("hello world!!!");

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
		char buffer[1024];
		int byteRecieved = recv(client, buffer, std::size(buffer) - 1, 0);

		if (byteRecieved == SOCKET_ERROR)
		{
			printf("recv failed\n");
			return -1;
		}
		if (byteRecieved == 0)
		{
			printf("Connection Closed\n");
		}
		else
		{
			buffer[byteRecieved] = '\0';
			if (buffer == Password)
			{
				int byteSent = send(client, "Access Granted", std::size(buffer) - 1, 0);
				printf("User Authenticated: %s\n", buffer);
			}
			else
			{
				int byteSent = send(client, "Access Denied", std::size(buffer) - 1, 0);
				printf("User Not Authenticated: %s\n", buffer);
			}
			int byteSent = send(client, buffer, std::size(buffer) -1, 0);
		}

		
		closesocket(client);
	}

	//WinSock Shutdown
	WSACleanup();

	return 0;
}