#include "GameObject.h"



GameObject::GameObject(bool sync)
	: m_destroyed{ false }
	, m_sync{ sync }
	, m_dirty{ false }
{

}
