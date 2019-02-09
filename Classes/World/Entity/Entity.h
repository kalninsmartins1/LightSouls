#pragma once

#include "LightSoulsTypes.h"

class AnimComponent;

enum class EntityType
{
	NONE,
	PLAYER,
	AIAGENT
};

class Entity: public cocos2d::Sprite
{
public:
	Entity();
	virtual ~Entity();

public:
	virtual EntityType		GetEntityType() const;
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
	bool					IsProcessing() const;
	
	unsigned int			GetId() const;
	bool					IsRunning() const;	
	bool					IsReadyToAttack() const;
	bool					HasEnoughtStamina(float amount) const;
	float					GetKnockBackStrenght() const;
	
	void			SetPhysicsBodyForceScale(float scale);
	void			SetBaseDamage(float baseDamage);
	void			SetBaseHealth(float baseHealth);
	void			SetBaseStamina(float baseStamina);
	void			SetBaseMoveSpeed(float moveSpeed);
	void			SetMoveDirection(const Vector2& direction);
	void			SetPhysicsBodySize(const cocos2d::Size& size);
	void			SetPhysicsBodyAnchor(const Vector2& achorPos);	
	void			SetCurrentMoveSpeed(float moveSpeed);
	void			SetStaminaRegenerateSpeed(float regenerateSpeed);
	void			SetStaminaRegenerateDelay(float regenerateDelay);
	void			SetKnockBackStrenght(float strenght);
	virtual void	setScale(float scaleX, float scaleY) override;
	

	void			ResetHealth();
	void			ResetStamina();
	void			ResetMoveSpeed();
	void			ResetIsTakingDamage();
	void			ConsumeStamina(float amount);
	void			TakeDamage(const Entity& attackingEntity);
	void			TakeDamage(float damage);
	void			StartAttacking();
	void			StopAttacking();
	void			ApplyKnockbackEffect(const Entity& attackingEntity);
	void			ApplyInstantSpeed(float speed);
	void			ApplyInstantSpeedInDirection(float speed, const Vector2& direction);
	virtual void	update(float deltaTime) override;

protected:
	virtual void DispatchOnHealthReduceEvent() = 0;
	virtual void DispatchOnStaminaChangedEvent() const = 0;
	virtual void DispatchOnGiveDamageEvent() const = 0;

	void		 OnEntityInitialized();

private:
	void		StartStaminaRegenerateDelayTimer();
	void		OnStaminaRegenerateDelayExpired();

	void		UpdateSortingLayer();
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
	bool m_isStaminaRegenerateDelayExpired;

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
	float m_staminaRegenerateDelay;
	float m_knockBackStrenght;
};

