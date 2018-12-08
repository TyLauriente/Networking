#pragma once

#include <Network.h>
#include <XEngine.h>

enum class BoardCommand
{
	MoveLeft,
	MoveRight,
	RotateLeft,
	RotateRight
};

enum class NetworkCommand
{
	StartGame,
	AddPlayer,
	UpdateBoard,
	AssignId,
	LoseGame,
	WinGame
};

class Command
{
public:
	Command() = default;
	
	void SetCommand(BoardCommand command) { m_commands.push_back(command); }

	bool CommandActive(BoardCommand command);

private:
	std::vector<BoardCommand> m_commands;
};

