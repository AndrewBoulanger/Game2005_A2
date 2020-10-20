#include "Box.h"
#include "TextureManager.h"


Box::Box()
{
	TextureManager::Instance()->load("../Assets/textures/lootbox.png","lootbox");

	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;
	m_angle = 0.0f;

	setType(TARGET);
}

Box::~Box()
= default;

void Box::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("lootbox", x, y - (getHeight()*.75), getWidth(), getHeight(), m_angle, 255, false);
}

void Box::update()
{
	m_move();
	m_checkBounds();
}

void Box::clean()
{
}

void Box::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}

void Box::m_checkBounds()
{
}

void Box::m_reset()
{
}
