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
			const Vector2& targetPosition = targetEntity->GetPos();
			const Vector2& ownerPosition = m_ownerEntity->GetPos();

			float curScaleX = abs(m_ownerEntity->getScaleX());
			if (targetPosition.GetX() > ownerPosition.GetX())
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