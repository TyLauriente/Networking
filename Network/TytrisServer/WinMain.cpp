#include "ServerManager.h"

int main()
{
	ServerManager::StaticInitialize();

	ServerManager::Get()->Run(8888);


	ServerManager::StaticTerminate();
	return 0;
}