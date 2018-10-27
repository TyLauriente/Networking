#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <Network.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	using namespace std;

	bool isServer = true;

	cout << "Choose Server or Client\n0. Client\n1. Server\n> ";
	cin >> isServer;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	Network::UDPSocket socket;
	int result = 0;

	if (isServer)// Server Code
	{
		const char* host = "255.255.255.255";
		uint32_t port = 8888;

		Network::SocketAddress socketAddress(host, port);
	
		if (!socket.Open())
		{
			cout << "Failed to open socket" << endl;
			WSACleanup();
			return 1;
		}
		
		if (!socket.SetBroadcast(true))
		{
			cout << "Failed to start broadcast" << endl;
			return 1;
		}


		int last = 1;
		int count = 1;
		cout << "Broadcasting Fibonacci" << endl;
		while (true)
		{
			int temp = last;
			last = count;
			count += temp;
			if (count > 5000)
			{
				last = 1;
				count = 1;
			}
			socket.SendTo(to_string(count).c_str(), sizeof(count), socketAddress);
			Sleep(1000);
		}
	}
	else// Client Code
	{
		Network::SocketAddress socketAddress;

		if (!socket.SetBroadcast(true))
		{
			cout << "Falied to set up broadcast" << endl;
			return 1;
		}

		if (!socket.Bind(socketAddress))
		{
			cout << "Failed to Bind" << endl;
			socket.Close();
			return 1;
		}
		
		char recvBuffer[1024];
		string message;

		while (true)
		{
			result = socket.ReceiveFrom(&recvBuffer, sizeof(recvBuffer), socketAddress);
			recvBuffer[result] = '\0';
			system("cls");
			message = recvBuffer;
			cout << "Current Message: " + message << endl;
		}

	}



	return 0;
}