#pragma once

#include "AttackComponent.h"


class LongSwordAttackComponent : public AttackComponent
{
public:
	virtual void attack(const Vector2& direction) override;
};