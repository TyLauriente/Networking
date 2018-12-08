#include "Command.h"

bool Command::CommandActive(BoardCommand command)
{
	for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
	{
		if ((*it) == command)
		{
			m_commands.erase(it);
			return true;
		}
	}
	return false;
}