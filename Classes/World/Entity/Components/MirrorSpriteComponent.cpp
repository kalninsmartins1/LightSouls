#include "MirrorSpriteComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"

NS_LIGHTSOULS_BEGIN

void MirrorSpriteComponent::setOwnerEntity(Entity* ownerEntity)
{
	m_ownerEntity = ownerEntity;
}

void MirrorSpriteComponent::update(float delta)
{
	// Mirror the sprite based on heading direction
	const Vector2 entityHeading = m_ownerEntity->GetHeading();
	const float absScaleX = abs(m_ownerEntity->getScaleX());
	if(entityHeading.x > 0)
	{		
		m_ownerEntity->setScaleX(absScaleX);
	}
	else if(entityHeading.x < 0)
	{
		m_ownerEntity->setScaleX(-absScaleX);
	}
}

NS_LIGHTSOULS_END
