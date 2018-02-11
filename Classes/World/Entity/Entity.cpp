#include "Entity.h"

Entity::Entity() :
	m_moveDirection(Vector2::ZERO),
	m_bIsRuning(false),
	m_bIsDodging(false),
	m_bIsAttacking(false),
	m_baseMoveSpeed(0),
	m_moveSpeed(0),
	m_dodgeSpeed(0),
	m_dodgeTime(0)
{
}

void Entity::setBaseMoveSpeed(float moveSpeed)
{
	m_baseMoveSpeed = moveSpeed;
	m_moveSpeed = m_baseMoveSpeed;
}

void Entity::setDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Entity::setDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
}

void Entity::setMoveDirection(const Vector2& direction)
{
	m_moveDirection = direction;
}

float Entity::getCurrentMoveSpeed() const
{	
	return m_moveSpeed;
}

Entity::Vector2 Entity::getHeading() const
{	
	return m_moveDirection;
}
