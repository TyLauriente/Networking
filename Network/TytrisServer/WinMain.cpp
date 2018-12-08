#include "ServerManager.h"

int main()
{
	int numberOfPlayers;
	ServerManager::StaticInitialize();

	std::cout << "This is the Tytris Server application\n";
	std::cout << "Enter amount of players: ";
	std::cin >> numberOfPlayers;

	if (numberOfPlayers > 3)
	{
		numberOfPlayers = 3;
	}
	else if (numberOfPlayers < 1)
	{
		numberOfPlayers = 1;
	}

	ServerManager::Get()->Run(8888, numberOfPlayers);


	ServerManager::StaticTerminate();
	return 0;
}