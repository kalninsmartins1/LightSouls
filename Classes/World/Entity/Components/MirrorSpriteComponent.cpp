#include "MirrorSpriteComponent.h"
#include "Utils/Utils.h"
#include "World/Entity/Entity.h"

MirrorSpriteComponent::MirrorSpriteComponent()
	: m_sensitivity(0.1f)
{

}

void MirrorSpriteComponent::Init(cc::Node& ownerEntity, float sensitivity)
{	
	m_ownerEntity = static_cast<Entity*>(&ownerEntity);
	m_sensitivity = sensitivity;
}

void MirrorSpriteComponent::update(float delta)
{
	if (m_ownerEntity != nullptr) 
	{
		// Mirror the sprite based on heading direction
		const Vector2 lookAt = m_ownerEntity->GetLookAtDirection();
		if (abs(lookAt.GetY()) < m_sensitivity)
		{
			const float absScaleX = abs(m_ownerEntity->getScaleX());
			if (lookAt.GetX() > m_sensitivity)
			{
				m_ownerEntity->setScaleX(absScaleX);
			}
			else if (lookAt.GetX() < -m_sensitivity)
			{
				m_ownerEntity->setScaleX(-absScaleX);
			}
		}		
	}	
}
