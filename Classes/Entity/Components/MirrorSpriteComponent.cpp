#include "MirrorSpriteComponent.h"
#include "Utils/Utils.h"
#include "Entity/Player/Player.h"

using namespace cocos2d;

void MirrorSpriteComponent::setOwner(cocos2d::Node* owner)
{
	m_pPlayer = (Player*)owner;
	if(m_pPlayer == nullptr)
	{
		cocos2d::log("MirrorSpriteComponent: Attached to [Node] that is not a [Sprite] !");
	}
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
