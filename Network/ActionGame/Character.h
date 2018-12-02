#pragma once

#include "GameObject.h"
#include "Command.h"


class Character : public GameObject
{
public:
	CLASS_ID(Character, 'CHAR');

	Character();


	void XInitialize() override;
	void Update(float deltaTime) override;
	void Draw() override;

	void Serialize(Network::StreamWriter& writer) const override;
	void Deserialize(Network::StreamReader& reader) override;

	void Apply(const Command& command)	{ m_command = command; }


	void SetPosition(const X::Math::Vector2& pos)	{ m_position = pos; }
	const X::Math::Vector2& GetPosition() const		{ return m_position; }

private:
	Command m_command;
	X::Math::Vector2 m_position;
	X::TextureId m_textureId;
};

