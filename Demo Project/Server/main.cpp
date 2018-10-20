#include <cstdio>
#include <WinSock2.h>
#include <string>
#include <cstdint>
#include <strsafe.h>
#include <iostream>
#include <vector>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")


int main(int argc, char* argv[])
{
	std::string path = "Data\\";
	std::string fileList;
	int index = 0;
	std::vector<std::string> files;
	
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER fileSize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;



	StringCchCopy(szDir, MAX_PATH, TEXT(path.c_str()));
	StringCchCat(szDir, MAX_PATH, TEXT("*"));

	

	hFind = FindFirstFile(szDir, &ffd);

	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			LARGE_INTEGER size;
			size.LowPart = ffd.nFileSizeLow;
			size.HighPart = ffd.nFileSizeHigh;

			fileList += std::to_string(index) + ". " + ffd.cFileName +
				" - " + std::to_string(size.QuadPart) + " bytes\n";
			files.push_back(ffd.cFileName);
			index++;
		}
	} while (FindNextFile(hFind, &ffd) != 0);


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

		char buffer[1024];

		int byteRecieved = recv(client, buffer, std::size(buffer) - 1, 0);

		buffer[byteRecieved] = '\0';
		int fileIndex = std::stoi(buffer); 
		std::string fullname = path + files[fileIndex];

		FILE* file = nullptr;
		fopen_s(&file, fullname.c_str(), "rb");

		fseek(file, 0L, SEEK_END);
		int fileSize = ftell(file);
		fseek(file, 0L, SEEK_SET);

		char* fileBuffer = (char*)malloc(fileSize * sizeof(char));
		fread(fileBuffer, fileSize, 1, file);
		fclose(file);

		int totalSent = 0;
		while (totalSent < fileSize)
		{
			int bytesSent = send(client, fileBuffer + totalSent, fileSize - totalSent, 0);
			totalSent += bytesSent;
		}

		send(client, fullname.c_str(), fullname.length(), 0);
		
		closesocket(client);
	}

	//WinSock Shutdown
	WSACleanup();

	return 0;
}