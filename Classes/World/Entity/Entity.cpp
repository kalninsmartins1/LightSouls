#include "Entity.h"

using namespace cocos2d;

Entity::Entity() :
	m_moveDirection(Vec2::ZERO),
	m_isRuning(false),
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
}

void Entity::setDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Entity::setDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
}

float Entity::getCurrentMoveSpeed() const
{
	return m_moveSpeed;
}

Vec2 Entity::getHeading() const
{
	return m_moveDirection;
}
