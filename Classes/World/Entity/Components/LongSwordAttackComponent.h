#pragma once

#include "AttackComponent.h"

class LongSwordAttackComponent : public AttackComponent
{
public:
	static LongSwordAttackComponent* create(float secondsBetweenAttacks);
	
	virtual void attack(const Vector2& direction) override;

private:
	LongSwordAttackComponent(float secondsBetweenAttacks);
};