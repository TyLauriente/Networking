#include "Character.h"



Character::Character()
	: GameObject{ true }
	, m_position{ 0.0f, 0.0f }
	, m_textureId{ 0 }
{

}

void Character::XInitialize()
{
	m_textureId = X::LoadTexture("Mario.png");
}

void Character::Update(float deltaTime)
{
	// Check if we are updating
	if (!m_command.Empty())
	{
		Dirty();
	}
	// Get how much time we want to apply the command this frame
	float commandTime = m_command.GetCommandTime(deltaTime);

	// Character control
	const float kMoveSpeed = 200.0f;
	if (m_command.MoveUp())
	{
		m_position.y -= kMoveSpeed * commandTime;
	}
	if (m_command.MoveDown())
	{
		m_position.y += kMoveSpeed * commandTime;
	}
	if (m_command.MoveLeft())
	{
		m_position.x -= kMoveSpeed * commandTime;
	}
	if (m_command.MoveRight())
	{
		m_position.x += kMoveSpeed * commandTime;
	}

	//Update command
	m_command.Update(deltaTime);
}

void Character::Draw()
{
	X::DrawSprite(m_textureId, m_position);
}

void Character::Serialize(Network::StreamWriter& writer) const
{
	writer.Write(m_position.x);
	writer.Write(m_position.y);
}

void Character::Deserialize(Network::StreamReader& reader)
{
	reader.Read(m_position.x);
	reader.Read(m_position.y);
}
