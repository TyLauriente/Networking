#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace Network;

void Network::Initialize()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void Network::Terminate()
{
	WSACleanup();
}