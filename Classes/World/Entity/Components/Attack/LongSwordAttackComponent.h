/*!
 * \Author	Martins Kalnins
 * \Date	19.06.2018
 * \Brief	Class for handling attacking with long sword.
 *				Responsible for:
 *					* Checking collision.
 *					* Triggering damage dealing.
 *					* Triggering correct animations.
*/
#pragma once

#include "GenericAttackComponent.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

class Entity;

using AttackFinishedCallback = std::function<void()>;

class LongSwordAttackComponent : public GenericAttackComponent
{
public:
	static LongSwordAttackComponent*	Create(float secondsBetweenAttacks, float attackRange, float paddingFromBody);	
	static const char*					GetHitOffsetPropName();

	void			SetDamageCheckDelay(const float damageCheckDelay);
	void			SetHitPointOffset(float hitPointOffset);
	virtual void	Attack(const Vector2& direction) override;
	void			SetAttackFinishCallback(const AttackFinishedCallback& callback);

protected:
	virtual void OnEntityHit(Entity* hitEntity) const override;

private:
	LongSwordAttackComponent(float secondsBetweenAttacks, float attackRange,
		float paddingFromBody);

private:
	bool OnAttackHit(cc::PhysicsWorld& world, 
		cc::PhysicsShape& physicsObject, void* metaData) const;
	void OnDamageCheck();
	void DispatchStartAttackEvent(const Vector2& direction);

private:
	static const char*	s_eventOnSlash;
	static const char*	s_eventOnLongSwordAttackStarted;
	static const char*	s_hitOffsetPropName;

	const float			m_paddingFromBody;
	float				m_damageCheckDelay;
	float				m_hitPointOffset;

	Vector2					m_lastAttackDirection;
	AttackFinishedCallback	m_attackFinishedCallback;
};
