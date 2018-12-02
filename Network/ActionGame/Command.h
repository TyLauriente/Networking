#pragma once

#include <Network.h>
#include <XEngine.h>

class Command
{
public:
	Command() = default;

	void SetDuration(float duration);
	float GetCommandTime(float deltaTime);

	void Update(float deltaTime);
	bool Empty() const;

	bool MoveUp() const;
	bool MoveDown() const;
	bool MoveRight() const;
	bool MoveLeft() const;

	void Serialize(Network::StreamWriter& writer) const;
	void Deserialize(Network::StreamReader& reader);


	enum TimedCommand
	{

		kUp		= 0x1 << 0, // 1
		kDown	= 0x1 << 1, // 2
		kRight	= 0x1 << 2, // 4
		kLeft	= 0x1 << 3  // 8
	};

	enum OneShotCommand
	{
		// Shoot = 0x1 << 0
	};

	
	float m_duration{ 0.0f };
	uint32_t m_timedCommand{ 0 };
	uint32_t m_oneShotCommand{ 0 };
};

