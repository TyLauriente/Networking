#pragma once


enum class BoardCommand
{
	MoveLeft,
	MoveRight,
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