#pragma once


enum class BoardCommand
{
	MoveLeft,
	MoveRight,
	MoveDown,
	RotateLeft,
	RotateRight,
	Hold,
	Place
};

enum class NetworkCommand
{
	SetupGame,
	AssignId,
	AddPlayer,
	StartGame,
	BoardCommand,
	SpawnShape,
	SendLines,
	PlayerDead,
	WinGame
};