#include "LookAtAITargetComponent.h"
#include "World/Entity/AI/AIAgentManager.h"

void LookAtAITargetComponent::SetOwnerEntity(Entity* ownerEntity)
{
	m_ownerEntity = ownerEntity;
}

void LookAtAITargetComponent::update(float delta)
{
	AIAgentManager* aiManager = AIAgentManager::GetInstance();
	if (aiManager != nullptr && m_ownerEntity != nullptr)
	{
		Entity* targetEntity = aiManager->GetTargetEntity();
		if (targetEntity != nullptr)
		{
			const Vector2& targetPosition = targetEntity->getPosition();
			const Vector2& ownerPosition = getOwner()->getPosition();

			float curScaleX = abs(m_ownerEntity->getScaleX());
			if (targetPosition.x > ownerPosition.x)
			{
				m_ownerEntity->setScaleX(-curScaleX);
			}
			else
			{
				m_ownerEntity->setScaleX(curScaleX);
			}
		}
	}
}