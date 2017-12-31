#include "MirrorSpriteComponent.h"
#include "Utils/Utils.h"
#include "Entity/Player/Player.h"

using namespace cocos2d;

void MirrorSpriteComponent::setPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

void MirrorSpriteComponent::update(float delta)
{
	// Mirror the sprite based on heading direction
	Vec2 playerHeading = m_pPlayer->getHeading();
	if(playerHeading.x > 0)
	{
		m_pPlayer->setScaleX(1);
	}
	else if(playerHeading.x < 0)
	{
		m_pPlayer->setScaleX(-1);
	}
}
