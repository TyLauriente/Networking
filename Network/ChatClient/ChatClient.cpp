#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#include <conio.h>
#pragma comment(lib, "Ws2_32.lib")


std::string typedSoFar;
std::mutex typedSoFarLock;

void SendMessagesToServer(SOCKET server)
{
	std::cout << "> ";
	while (true)
	{
		char c = ' ';
		c = _getch();
		if (c != '\0')
		{
			if (c == '\r')//Enter key is pressed
			{
				typedSoFarLock.lock();
				int sent = static_cast<int>(send(server, typedSoFar.c_str(), static_cast<int>(typedSoFar.size()), 0));
				typedSoFar = "";
				typedSoFarLock.unlock();
				if (sent == SOCKET_ERROR)
				{
					std::cout << "Failed to send message to server\n";
					return;
				}
				std::cout << "\n> ";
			}
			else if (c == '\b')
			{
				if (typedSoFar.size() > 0)
				{
					typedSoFar = typedSoFar.substr(0, typedSoFar.size() - 1);
					std::cout << "\b \b";
				}
			}
			else
			{
				typedSoFarLock.lock();
				std::cout << c;
				typedSoFar += c;
				typedSoFarLock.unlock();
			}
		}
	}
}

void ReceievMessagesFromServer(SOCKET server)
{
	char buffer[1024];

	while (true)
	{
		int received = recv(server, buffer, sizeof(buffer) - 1, 0);
		if (received == SOCKET_ERROR)
		{
			std::cout << "Failed to receive messages\n";
			break;
		}
		buffer[received] = '\0';

		for (int index = 0; index < typedSoFar.length() + 2; ++index)
		{
			std::cout << '\b';
		}
		std::cout << buffer << std::endl;
		if (typedSoFar.length() > 0)
		{
			std::cout << "> ";
		}
	}
}

int main()
{
	std::string ip;
	uint16_t port = 8888;

	std::cout << "Enter Hosts IP: ";
	std::cin >> ip;


	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET host = socket(AF_INET, SOCK_STREAM, NULL);

	
	SOCKADDR_IN serverInfo;
	inet_pton(AF_INET, ip.c_str(), &serverInfo.sin_addr.S_un.S_addr);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(port);
	
	int connectionToServer = connect(host, (SOCKADDR*)&serverInfo, sizeof(serverInfo));
	if (connectionToServer == SOCKET_ERROR)
	{
		std::cout << "Failed to connect to server\n";
		closesocket(host);
		WSACleanup();
		system("pause");
		return 0;
	}

	std::cout << "Connected to Server\n";

	char buffer[1024];

	int received = static_cast<int>(recv(host, buffer, static_cast<int>(std::size(buffer)) - 1, 0));

	if (received == SOCKET_ERROR)
	{
		std::cout << "Failed to recv welcome message\n";
		closesocket(host);
		WSACleanup();
		system("pause");
		return 0;
	}
	buffer[received] = '\0';

	std::cout << buffer;
	std::string name;
	std::cin >> name;
	int sent = static_cast<int>(send(host, name.c_str(), static_cast<int>(name.length()), 0));
	if (sent == SOCKET_ERROR)
	{
		std::cout << "Failed to send name to server\n";
		closesocket(host);
		WSACleanup();
		system("pause");
		return 0;
	}

	std::thread receiveMessages= std::thread(ReceievMessagesFromServer, host);
	std::thread sendMessages = std::thread(SendMessagesToServer, host);

	sendMessages.join();
	receiveMessages.join();

	closesocket(host);
	WSACleanup();
	system("pause");
}