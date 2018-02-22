#include "Entity.h"

unsigned int Entity::uniqueId = 0;

Entity::Entity() :
	m_moveDirection(Vector2::ZERO),
	m_bIsRuning(false),
	m_bIsDodging(false),
	m_bIsAttacking(false),
	m_baseMoveSpeed(0),
	m_baseHealth(0),
	m_baseDamage(0),
	m_health(0),
	m_damage(0),
	m_moveSpeed(0),
	m_dodgeSpeed(0),
	m_dodgeTime(0),
	m_Id(uniqueId++)
{
}

void Entity::setBaseDamage(float baseDamage)
{
	m_baseDamage = baseDamage;
	m_damage = baseDamage;
}

void Entity::setBaseHealth(float baseHealth)
{
	m_baseHealth = baseHealth;
	m_health = baseHealth;
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

void Entity::setPhysicsBodySize(cocos2d::Size size)
{
	m_PhysicsBodySize = size;
}

void Entity::TakeDamage(float damage)
{
	if(m_health > 0)
	{
		m_health -= damage;
		if(m_health < 0)
		{
			m_health = 0;
		}
	}
}

float Entity::getCurrentMoveSpeed() const
{	
	return m_moveSpeed;
}

Entity::Vector2 Entity::getHeading() const
{	
	return m_moveDirection;
}

cocos2d::Size Entity::getPhysicsBodySize() const
{
	return m_PhysicsBodySize;
}

float Entity::getHealth() const
{
	return m_health;
}

float Entity::getDamage() const
{
	return m_damage;
}

unsigned Entity::getId() const
{
	return m_Id;
}
