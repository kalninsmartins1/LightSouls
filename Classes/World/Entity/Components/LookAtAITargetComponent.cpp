#include "LookAtAITargetComponent.h"
#include "World/Entity/AI/AIAgentManager.h"

void LookAtAITargetComponent::SetOwnerEntity(cc::Node& ownerEntity)
{
	m_ownerEntity = static_cast<Entity*>(&ownerEntity);	
	CCASSERT(m_ownerEntity != nullptr, "Error: Component has been added to node that is not an entity !");
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