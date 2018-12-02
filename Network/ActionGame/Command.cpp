#include "Command.h"


void Command::SetDuration(float duration)
{
	m_duration = duration;
}

float Command::GetCommandTime(float deltaTime)
{
	return X::Math::Min(m_duration, deltaTime);
}

void Command::Update(float deltaTime)
{
	//Timed commands will last duration long
	m_duration -= deltaTime;
	if (m_duration <= 0.0f)
	{
		m_duration = 0.0f;
		m_timedCommand = 0;
	}

	//Clear one shot flags
	m_oneShotCommand = 0;
}

bool Command::Empty() const
{
	return m_timedCommand == 0 && m_oneShotCommand == 0;
}

bool Command::MoveUp() const
{
	return (m_timedCommand & kUp) != 0;
}

bool Command::MoveDown() const
{
	return (m_timedCommand & kDown) != 0;
}

bool Command::MoveRight() const
{
	return (m_timedCommand & kRight) != 0;
}

bool Command::MoveLeft() const
{
	return (m_timedCommand & kLeft) != 0;
}


void Command::Serialize(Network::StreamWriter& writer) const
{
	writer.Write(m_duration);
	writer.Write(m_timedCommand);
	writer.Write(m_oneShotCommand);
}

void Command::Deserialize(Network::StreamReader& reader)
{
	reader.Read(m_duration);
	reader.Read(m_timedCommand);
	reader.Read(m_oneShotCommand);
}
