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
	const Vec2 playerHeading = m_pEntity->getHeading();
	if(playerHeading.x > 0)
	{
		m_pEntity->setScaleX(1);
	}
	else if(playerHeading.x < 0)
	{
		m_pEntity->setScaleX(-1);
	}
}
