#pragma once


enum class BoardCommand
{
	MoveLeft,
	MoveRight,
	MoveDown,
	RotateLeft,
	RotateRight
};

enum class NetworkCommand
{
	SetupGame,
	AddPlayer,
	StartGame,
	TickDown,
	BoardCommand,
	SpawnShape,
	AssignId,
	LoseGame,
	WinGame
};