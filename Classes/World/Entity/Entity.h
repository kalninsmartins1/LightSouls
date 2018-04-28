#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Entity: public cocos2d::Sprite
{
public:
	Entity();
	virtual ~Entity();

public:
	float					GetCurrentMoveSpeed() const;
	const Vector2&			GetHeading() const;
	const cocos2d::Size&	GetPhysicsBodySizeScaled() const;
	float					GetHealth() const;
	float					GetDamage() const;
	float					GetDodgeTime() const;
	float					GetDodgeSpeed() const;
	float					GetCurrentHealth() const;
	float					GetMaxHealth() const;
	float					GetPhysicsBodyForceScale() const;
	unsigned int			GetId() const;
	bool					IsRunning() const;
	bool					IsDodging() const;
	bool					IsAttacking() const;

	void SetPhysicsBodyForceScale(float scale);
	void SetBaseDamage(float baseDamage);
	void SetBaseHealth(float baseHealth);
	void SetBaseMoveSpeed(float moveSpeed);
	void SetDodgeSpeed(float dodgeSpeed);
	void SetDodgeTime(float dodgeTime);	
	void SetMoveDirection(const Vector2& direction);
	void SetPhysicsBodySize(const cocos2d::Size& size);
	void SetPhysicsBodyAnchor(const Vector2& achorPos);
	
	void TakeDamage(float damage);
	void StartDodging();
	void StopDodging();
	void StartAttacking();
	void StopAttacking();

	virtual void update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthChangedEvent();
	void		 OnEntityInitialized();

private:
	void		SetCurrentMoveSpeed(float moveSpeed);
	void		Move();	

private:	
	static unsigned int			s_uniqueId;
	unsigned int				m_id;
		
	Vector2					m_moveDirection;
	cocos2d::Size			m_physicsBodyScaledSize;	
	
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
	float m_physicsBodyForceScale;
};

NS_LIGHTSOULS_END
