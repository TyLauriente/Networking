#include <iostream>
#include "ServerManager.h"


int main()
{
	GameObjectManager::StaticInitialize();
	ServerManager::StaticInitialize();

	GameObjectManager::Get()->GetFactory().Register<Character>();

	
	ServerManager::Get()->Run(8888);

	GameObjectManager::StaticTerminate();
	ServerManager::StaticTerminate();
	system("pause");
	return 0;
}