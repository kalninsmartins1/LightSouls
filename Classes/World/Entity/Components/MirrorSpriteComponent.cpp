#include "MirrorSpriteComponent.h"
#include "Utils/Utils.h"
#include "../Player/Player.h"

using namespace cocos2d;

void MirrorSpriteComponent::setOwnerEntity(Entity* pEntity)
{
	m_pEntity = pEntity;
}

void MirrorSpriteComponent::update(float delta)
{
	// Mirror the sprite based on heading direction
	const Vec2 entityHeading = m_pEntity->getHeading();
	const float absScaleX = abs(m_pEntity->getScaleX());
	if(entityHeading.x > 0)
	{		
		m_pEntity->setScaleX(absScaleX);
	}
	else if(entityHeading.x < 0)
	{
		m_pEntity->setScaleX(-absScaleX);
	}
}
