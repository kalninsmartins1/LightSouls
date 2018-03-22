#pragma once

#include "2d/CCSprite.h"

class Entity: public cocos2d::Sprite
{
public:
	using Vector2 = cocos2d::Vec2;
	using String = std::string;

	Entity();
	virtual ~Entity();

	float					GetCurrentMoveSpeed() const;
	const Vector2&			GetHeading() const;
	const cocos2d::Size&	GetPhysicsBodySize() const;
	float					GetHealth() const;
	float					GetDamage() const;
	float					GetDodgeTime() const;
	float					GetDodgeSpeed() const;
	unsigned int			GetId() const;
	bool					IsRunning() const;
	bool					IsDodging() const;
	bool					IsAttacking() const;

	void SetBaseDamage(float baseDamage);
	void SetBaseHealth(float baseHealth);
	void SetBaseMoveSpeed(float moveSpeed);
	void SetDodgeSpeed(float dodgeSpeed);
	void SetDodgeTime(float dodgeTime);	
	void SetMoveDirection(const Vector2& direction);
	void SetPhysicsBodySize(cocos2d::Size size);
	
	void TakeDamage(float damage);
	void StartDodging();
	void StopDodging();
	void StartAttacking();
	void StopAttacking();

	virtual void update(float deltaTime) override;
private:
	static unsigned int s_uniqueId;
	unsigned int		m_Id;

	Vector2			m_moveDirection;
	cocos2d::Size	m_physicsBodySize;

	bool m_isRuning;
	bool m_isDodging;
	bool m_isAttacking;

	float m_baseMoveSpeed;
	float m_baseHealth;
	float m_baseDamage;
	float m_health;
	float m_damage;
	float m_moveSpeed;
	float m_dodgeSpeed;
	float m_dodgeTime;
};
