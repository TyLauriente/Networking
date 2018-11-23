#pragma once

#include <XEngine.h>

//Consts
static const float DEFAULT_MAX_VELOCITY = 400.0f;
static const float DEFAULT_MASS = 5.0f;
static const float DEFAULT_ACCELERATION = 10.0f;
static const char* DEFAULT_TEXTURE_ID = "Ball.png";

enum class GravityDirection
{
	Up,
	Down,
	Left,
	Right
};


class Ball
{
public:
	Ball(float& gravityForce, X::Math::Vector2 startPos);
	~Ball();

	void XInitialize();

	void Update(float deltaTime);

	void Render();



private:
	float DeltaTime{ 0.0f };
	float& GravityForce;
	float ScreenWidth;
	float ScreenHeight;
	float SpriteWidth;
	float SpriteHeight;

	X::TextureId m_textureId{ 0 };
	GravityDirection m_gravityDirectionY{ GravityDirection::Down };
	GravityDirection m_gravityDirectionX{ GravityDirection::Right };
	X::Math::Vector2 m_position{ 0.0f, 0.0f };
	X::Math::Vector2 m_velocity{ 0.0f, 0.0f };
	float m_mass{ DEFAULT_MASS };
	float m_maxVelocity{ DEFAULT_MAX_VELOCITY };
	float m_acceleration{ DEFAULT_ACCELERATION };


	//Private Functions
	void ApplyGravity();
	void UpdatePosition();
	void UpdateGravityDirection();
};

