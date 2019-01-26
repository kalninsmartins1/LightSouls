#pragma once

#include "LightSoulsTypes.h"

class Entity;

class MirrorSpriteComponent: public cocos2d::Component
{
public:
	MirrorSpriteComponent();

public:
			CREATE_FUNC(MirrorSpriteComponent);
	void	Init(Entity& ownerEntity, float sensitivity);

	virtual void update(float delta) override;

private:
	Entity*			m_ownerEntity;
	float			m_sensitivity;
};


