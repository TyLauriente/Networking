#include "Ball.h"



Ball::Ball(float& gravityForce, X::Math::Vector2 startPos)
	: GravityForce{ gravityForce }
	, m_position{ startPos }
{

}


Ball::~Ball()
{

}


void Ball::XInitialize()
{
	m_textureId = X::LoadTexture(DEFAULT_TEXTURE_ID);
	SpriteWidth = static_cast<float>(X::GetSpriteWidth(m_textureId));
	SpriteHeight = static_cast<float>(X::GetSpriteHeight(m_textureId));
	ScreenWidth = static_cast<float>(X::GetScreenWidth());
	ScreenHeight = static_cast<float>(X::GetScreenHeight());

	if(m_position == X::Math::Vector2{0.0f, 0.0f})
	{
		m_position = { (ScreenWidth*0.5f), (ScreenHeight* 0.5f) };
	}
}

void Ball::Update(float deltaTime)
{
	DeltaTime = deltaTime;
	ApplyGravity();
	UpdatePosition();
	UpdateGravityDirection();
	std::string text = "X: " + std::to_string(m_velocity.x) + " Y: " + std::to_string(m_velocity.y);
	X::DrawScreenText(text.c_str(), ScreenWidth, 100.0f, 24.0f, X::Math::Vector4::Green());

	if(X::IsKeyPressed(X::Keys::D))
	{
		m_position.x = m_position.x;
	}
}

void Ball::Render()
{
	X::DrawSprite(m_textureId, m_position);
}

void Ball::ApplyGravity()
{
	const float gravityForce{ (GravityForce * DeltaTime) / m_mass };


	if(m_gravityDirectionY == GravityDirection::Up && (m_velocity.y - gravityForce) >= -m_maxVelocity && m_position.y > (SpriteHeight * 0.5f))
	{
		m_velocity.y -= gravityForce;
	}
	else if(m_gravityDirectionY == GravityDirection::Down && (m_velocity.y + gravityForce) <= m_maxVelocity && m_position.y < (ScreenHeight - (SpriteHeight * 0.5)))
	{
		m_velocity.y += gravityForce;
	}
	if(m_gravityDirectionX == GravityDirection::Left && (m_velocity.x - gravityForce) >= -m_maxVelocity && m_position.x > (SpriteWidth * 0.5f))
	{
		m_velocity.x -= gravityForce;
	}
	else if(m_gravityDirectionX == GravityDirection::Right && (m_velocity.x + gravityForce) <= m_maxVelocity && m_position.x < (ScreenWidth - (SpriteWidth * 0.5f)))
	{
		m_velocity.x += gravityForce;
	}
}

void Ball::UpdatePosition()
{
	//X Velocity
	if (m_velocity.x < 0.0f && (m_position.x - (m_velocity.x * DeltaTime)) < (SpriteWidth * 0.5f))
	{
		m_position.x = (SpriteWidth * 0.5f);
		m_velocity.x = 0.0f;
	}
	else if (m_velocity.x > 0.0f && (m_position.x + (m_velocity.x * DeltaTime)) > (ScreenWidth - (SpriteWidth * 0.5f)))
	{
		m_position.x = (ScreenWidth - (SpriteWidth * 0.5f));
		m_velocity.x = 0.0f;
	}
	else
	{
		m_position.x += m_velocity.x * DeltaTime;
	}
	//Y Velocity
	if (m_velocity.y < 0.0f && (m_position.y - (m_velocity.y * DeltaTime)) < (SpriteHeight * 0.5f))
	{
		m_position.y = (SpriteHeight * 0.5f);
		m_velocity.y = 0.0f;
	}
	else if (m_velocity.y > 0.0f && (m_position.y + (m_velocity.y * DeltaTime)) > (ScreenHeight - (SpriteHeight * 0.5f)))
	{
		m_position.y = (ScreenHeight - (SpriteHeight * 0.5f));
		m_velocity.y = 0.0f;
	}
	else
	{
		m_position.y += m_velocity.y * DeltaTime;
	}
}

void Ball::UpdateGravityDirection()
{
	if(X::IsKeyPressed(X::Keys::UP))
	{
		m_gravityDirectionY = GravityDirection::Up;
	}
	else if(X::IsKeyPressed(X::Keys::DOWN))
	{
		m_gravityDirectionY = GravityDirection::Down;
	}
	else if(X::IsKeyPressed(X::Keys::RIGHT))
	{
		m_gravityDirectionX = GravityDirection::Right;
	}
	else if(X::IsKeyPressed(X::Keys::LEFT))
	{
		m_gravityDirectionX = GravityDirection::Left;
	}
}
