#include "Entity.h"

unsigned int Entity::s_uniqueId = 0;

Entity::Entity() :
	m_moveDirection(Vector2::ZERO),
	m_isRuning(false),
	m_isDodging(false),
	m_isAttacking(false),
	m_baseMoveSpeed(0),
	m_baseHealth(0),
	m_baseDamage(0),
	m_health(0),
	m_damage(0),
	m_moveSpeed(0),
	m_dodgeSpeed(0),
	m_dodgeTime(0),
	m_Id(s_uniqueId++)
{
}

Entity::~Entity()
{

}

void Entity::SetBaseDamage(float baseDamage)
{
	m_baseDamage = baseDamage;
	m_damage = baseDamage;
}

void Entity::SetBaseHealth(float baseHealth)
{
	m_baseHealth = baseHealth;
	m_health = baseHealth;
}

void Entity::SetBaseMoveSpeed(float moveSpeed)
{
	m_baseMoveSpeed = moveSpeed;
	m_moveSpeed = m_baseMoveSpeed;
}

void Entity::SetDodgeSpeed(float dodgeSpeed)
{
	m_dodgeSpeed = dodgeSpeed;
}

void Entity::SetDodgeTime(float dodgeTime)
{
	m_dodgeTime = dodgeTime;
}

void Entity::SetMoveDirection(const Vector2& direction)
{
	m_moveDirection = direction;
}

void Entity::SetPhysicsBodySize(cocos2d::Size size)
{
	m_physicsBodySize = size;
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

void Entity::StartDodging()
{
	m_isDodging = true;
	m_moveSpeed = m_dodgeSpeed;
}

void Entity::StopDodging()
{
	m_isDodging = false;
	m_moveSpeed = m_baseMoveSpeed;
}

void Entity::StartAttacking()
{
	m_isAttacking = true;
}

void Entity::StopAttacking()
{
	m_isAttacking = false;
}

void Entity::update(float deltaTime)
{
	Sprite::update(deltaTime);
	m_isRuning = m_moveDirection.lengthSquared() > 0;
}

float Entity::GetCurrentMoveSpeed() const
{	
	return m_moveSpeed;
}

const Entity::Vector2& Entity::GetHeading() const
{	
	return m_moveDirection;
}

const cocos2d::Size& Entity::GetPhysicsBodySize() const
{
	return m_physicsBodySize;
}

float Entity::GetHealth() const
{
	return m_health;
}

float Entity::GetDamage() const
{
	return m_damage;
}

float Entity::GetDodgeTime() const
{
	return m_dodgeTime;
}

float Entity::GetDodgeSpeed() const
{
	return m_dodgeSpeed;
}

unsigned Entity::GetId() const
{
	return m_Id;
}

bool Entity::IsRunning() const
{
	return m_isRuning;
}

bool Entity::IsDodging() const
{
	return m_isDodging;
}

bool Entity::IsAttacking() const
{
	return m_isAttacking;
}