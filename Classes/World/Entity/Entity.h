#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/Math/Vector2.h"

class AnimComponent;
class BaseEventData;
class String;


class Entity: public cc::Sprite
{
public:
	Entity();
	virtual ~Entity();

public:
	float					GetCurrentMoveSpeed() const;
	const Vector2&			GetMoveDirection() const;
	const Vector2&			GetLookAtDirection() const;
	const Vector2&			GetPos() const;
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
	
	unsigned int				GetId() const;
	bool						IsRunning() const;	
	bool						IsReadyToAttack() const;
	bool						HasEnoughtStamina(float amount) const;
	float						GetKnockBackStrenght() const;
	float						GetTimeModifier() const;
	
	void			SetPhysicsBodyForceScale(float scale);
	void			SetBaseDamage(float baseDamage);
	void			SetBaseHealth(float baseHealth);
	void			SetBaseStamina(float baseStamina);
	void			SetBaseMoveSpeed(float moveSpeed);
	void			SetMoveDirection(const Vector2& direction);
	void			SetLookAtDirection(const Vector2& direction);
	void			SetPhysicsBodySize(const cocos2d::Size& size);
	void			SetPhysicsBodyAnchor(const Vector2& achorPos);	
	void			SetCurrentMoveSpeed(float moveSpeed);
	void			SetStaminaRegenerateSpeed(float regenerateSpeed);
	void			SetStaminaRegenerateDelay(float regenerateDelay);
	void			SetKnockBackStrenght(float strenght);
	void			SetTimeModifier(float timeModifier);
	void			SetPos(const Vector2& pos);
	virtual void	setScale(float scaleX, float scaleY) override;

	void			SwitchPhysicsBodyEnabled(bool isEnabled);
	void			ResetHealth();
	void			ResetStamina();
	void			ResetMoveSpeed();
	void			PrepareForRespawn();	
	void			ConsumeStamina(float amount);
	void			TakeDamage(const Entity& attackingEntity);
	void			TakeDamage(float damage);
	
	void			DispatchEvent(const char* eventType) const;
	void			DispatchEvent(const char* eventType, BaseEventData* eventData) const;
	void			ApplyKnockbackEffect(const Entity& attackingEntity);
	void			ApplyInstantSpeed(float speed);
	void			ApplyInstantSpeedInDirection(float speed, const Vector2& direction);	
	void			UpdatePos();
	virtual void	Update(float deltaTime);

protected:
	virtual void DispatchOnHealthReduceEvent() = 0;
	virtual void DispatchOnStaminaChangedEvent() const = 0;
	virtual void DispatchOnGiveDamageEvent() const = 0;	
	virtual void DispatchOnDisappeared() const = 0;
	void		 OnEntityInitialized();	

private:
	void		StartStaminaRegenerateDelayTimer();
	void		OnStaminaRegenerateDelayExpired();
	void		OnDisappeared();
	void		PlayHurtAnim();
	void		PlayDissapearAnim();
	void		RegisterToEvents();
	void		OnHealthReachedZero();

	void		UpdateSortingLayer();
	void		Move();	
	void		RegenerateStamina(float regenerateSpeedASecond);
	void		OnDamageTaken();
	void		OnGameSpeedModificationStarted();
	void		OnGameSpeedModificationEnded();

private:	
	static unsigned int			s_uniqueId;
	unsigned int				m_id;

	AnimComponent*				m_animComponent;
	Vector2						m_pos;
	Vector2						m_moveDirection;
	Vector2						m_lookAtDirection;
	cc::Size					m_physicsBodyScaledSize;

	bool m_isRuning;
	bool m_isTakingDamage;
	bool m_isDisappearing;
	bool m_isStaminaRegenerateDelayExpired;
	bool m_isGameSpeedBeingModified;

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

