#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class AnimComponent;

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
	float					GetCurrentHealth() const;
	float					GetCurrentStamina() const;
	float					GetMaxHealth() const;
	float					GetMaxStamina() const;
	float					GetPhysicsBodyForceScale() const;
	AnimComponent*			GetAnimComponent() const;
	
	unsigned int			GetId() const;
	bool					IsRunning() const;	
	bool					IsReadyToAttack() const;
	bool					HasEnoughtStamina(float amount);
	
	void SetPhysicsBodyForceScale(float scale);
	void SetBaseDamage(float baseDamage);
	void SetBaseHealth(float baseHealth);
	void SetBaseStamina(float baseStamina);
	void SetBaseMoveSpeed(float moveSpeed);
	void SetMoveDirection(const Vector2& direction);
	void SetPhysicsBodySize(const cocos2d::Size& size);
	void SetPhysicsBodyAnchor(const Vector2& achorPos);	
	void SetCurrentMoveSpeed(float moveSpeed);
	void SetStaminaRegenerateSpeed(float regenerateSpeed);

	void ResetMoveSpeed();
	void ConsumeStamina(float amount);
	void TakeDamage(float damage);
	void StartAttacking();
	void StopAttacking();

	virtual void update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthChangedEvent();
	virtual void DispatchOnStaminaChangedEvent();
	
	void		 OnEntityInitialized();

private:	
	void		Move();	
	void		RegenerateStamina(float regenerateSpeedASecond);
	void		OnDamageTaken();

private:	
	static unsigned int			s_uniqueId;
	unsigned int				m_id;
	
	AnimComponent*			m_animComponent;
	Vector2					m_moveDirection;
	cocos2d::Size			m_physicsBodyScaledSize;
	
	bool m_isRuning;
	bool m_isAttacking;
	bool m_isTakingDamage;
	
	float m_baseMoveSpeed;
	float m_baseHealth;
	float m_baseDamage;
	float m_baseStamina;
	float m_stamina;
	float m_health;
	float m_damage;
	float m_moveSpeed;
	float m_physicsBodyForceScale;
	float m_staminaRegenerateSpeed;
};

NS_LIGHTSOULS_END
