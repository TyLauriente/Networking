#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")


struct Message
{
	SOCKET sender;
	std::string text;
};

std::vector<Message> messageQueue;
std::vector<SOCKET> clients;
std::vector<std::thread> threads;
std::mutex messageQueueLock;
std::mutex clientsLock;


void ManageClient(SOCKET client)
{
	std::string welcomeMessage = "Enter your name: ";
	std::string name;
	char buffer[1024];
	int sent = send(client, welcomeMessage.c_str(), welcomeMessage.size(), 0);
	if (sent == SOCKET_ERROR)
	{
		std::cout << "SERVER: Failed to send welcome message to client" << std::endl;
		return;
	}
	int received = recv(client, buffer, sizeof(buffer) - 1, 0);
	if (received == SOCKET_ERROR)
	{
		std::cout << "SERVER: Failed to recieve name from client" << std::endl;
		return;
	}
	buffer[received] = '\0';
	name = buffer;
	std::cout << "SERVER: " << name << " has joined the chat\n";
	messageQueueLock.lock();
	messageQueue.push_back(Message({ client, ("\nServer: " + name + " has joined the chat") }));
	messageQueueLock.unlock();

	

	while (true)
	{
		received = recv(client, buffer, sizeof(buffer) - 1, 0);
		if (received == SOCKET_ERROR)
		{
			std::cout << "SERVER: " << name << " has disconected\n";
			messageQueueLock.lock();
			messageQueue.push_back(Message({ client, ("\nServer: " + name + " has disconected\n") }));
			messageQueueLock.unlock();
			break;
		}
		buffer[received] = '\0';
		std::cout << name + ": " << buffer << std::endl;
		messageQueueLock.lock();
		messageQueue.push_back(Message{ client, (name + ": " + buffer) });
		messageQueueLock.unlock();
	}

	clientsLock.lock();
	for (auto it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it == client)
		{
			clients.erase(it);
			break;
		}
	}
	clientsLock.unlock();
	closesocket(client);
}

void MessageDispatcher()
{
	while (true)
	{
		if (!messageQueue.empty() && !clients.empty())
		{
			clientsLock.lock();
			messageQueueLock.lock();
			for (auto client : clients)
			{
				for (const auto message : messageQueue)
				{
					if (message.sender != client)
					{
						send(client, message.text.c_str(), message.text.length(), 0);
					}
				}
			}
			messageQueue.clear();
			clientsLock.unlock();
			messageQueueLock.unlock();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


int main()
{
	uint32_t port = 8888;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET listenerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	SOCKADDR_IN clientInfo;
	clientInfo.sin_family = AF_INET;
	clientInfo.sin_addr.S_un.S_addr= INADDR_ANY;
	clientInfo.sin_port = htons(port);
	std::cout<<"SERVER: Listeing on port " << port << "....\n";
	
	
	bind(listenerSocket, (SOCKADDR*)&clientInfo, sizeof(sockaddr));
	listen(listenerSocket, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	std::thread messageDispatcher = std::thread(MessageDispatcher);


	while (true)
	{
		SOCKET clientSocket = accept(listenerSocket, (sockaddr*)&client, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "SERVER: Failed to connect to clients\n";
			system("pause");
			return 0;
		}

		char host[NI_MAXHOST];
		char service[NI_MAXSERV];

		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXSERV);
		
		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			std::cout << "SERVER: connected on port " << service << std::endl;
		}
		else
		{
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << "SERVER: " << host << " connected on port " << ntohs(client.sin_port) << std::endl;
		}



		clientsLock.lock();
		clients.push_back(clientSocket);
		std::cout << "Client number " << clients.size() << std::endl << std::endl;
		clientsLock.unlock();
		threads.push_back(std::thread(ManageClient, clientSocket));
	}
}