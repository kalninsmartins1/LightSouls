#include "SpawnPoint.h"
#include "AIAgent.h"
#include "AIAgentManager.h"
#include "Utils/Utils.h"
#include "Classes/Core/Events/BaseEventData.h"

SpawnPoint::SpawnPoint(const SpawnPointConfig& config)
	: m_respawnIndexQueue()
	, m_spawnedAgents()
	, m_config(config)
	, m_curRowCount(0)
	, m_lastAgentPos(Vector2::GetZero())	
	, m_curRespawnIndex(-1)
{
	const Vector2 pos = config.GetPosition();
	setPosition(pos.GetX(), pos.GetY());

	cc::EventDispatcher* dispatcher = getEventDispatcher();
	if (dispatcher != nullptr)
	{
		dispatcher->addCustomEventListener(AIAgent::GetEventOnDisappeared().GetCStr(),
			CC_CALLBACK_1(SpawnPoint::OnAgentDisappeared, this));
	}
}

void SpawnPoint::GetNextSpawnPosition(const cocos2d::Size& agentSize, Vector2& outPosition)
{
	if (m_lastAgentPos.GetX() == 0 && m_lastAgentPos.GetY() == 0)
	{
		float x, y;
		getPosition(&x, &y);
		outPosition.Set(x, y);
		m_curRowCount++;
	}
	else if(m_curRowCount < m_config.GetNumAgentsInRow())
	{
		outPosition = m_lastAgentPos + Vector2::GetOneX() * 
			(agentSize.width + m_config.GetAgentColumnPadding());
		m_curRowCount++;
	}
	else
	{
		m_curRowCount = 0;
		outPosition = Vector2(getPositionX(), m_lastAgentPos.GetY() + 
			(agentSize.height + m_config.GetAgentRowPadding()));
	}
	m_lastAgentPos = outPosition;
}

int SpawnPoint::FindAgentIndexById(int id)
{
	int keyIndex = -1;
	int index = 0;
	for (auto& agent : m_spawnedAgents)
	{
		if (agent->GetId() == id)
		{
			keyIndex = index;
		}
		++index;
	}

	return keyIndex;
}

SpawnPoint* SpawnPoint::Create(const SpawnPointConfig& config)
{
	SpawnPoint* spawnPoint = new (std::nothrow) SpawnPoint(config);
	if (spawnPoint != nullptr)
	{
		spawnPoint->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(spawnPoint);
	}

	return spawnPoint;
}

void SpawnPoint::SetTimeModifier(float timeModifier)
{
	for (auto agent : m_spawnedAgents)
	{
		agent->SetTimeModifier(timeModifier);
	}
}

void SpawnPoint::Update(float deltaTime)
{
	for (auto agent : m_spawnedAgents)
	{
		agent->Update(deltaTime);
	}

	ManageAgentRespawning();
}

void SpawnPoint::DespawnAgent(unsigned int agentIndex)
{
	AIAgent* agent = m_spawnedAgents[agentIndex];
	if (agent != nullptr && agent->isVisible())
	{
		SetAgentEnabled(agentIndex, false);
		m_respawnIndexQueue.push(agentIndex);
	}	
}

void SpawnPoint::SpawnAgent(bool isRespawn)
{
	String pathToXML;
	AIAgentManager::GetInstance()->GetPathToAgentType(m_config.GetAgentType(), pathToXML);

	if (!pathToXML.IsEmpty())
	{
		AIAgent* agent = AIAgent::Create(pathToXML);
		m_spawnedAgents.push_back(agent);

		// Set agents camera mask same as world layer so that it is visible to camera
		Node* worldLayer = getParent();
		agent->setCameraMask(worldLayer->getCameraMask());
		worldLayer->addChild(agent);

		// Actors spawn position is also his base position
		Vector2 basePositon;
		GetNextSpawnPosition(agent->GetPhysicsBodySizeScaled(), basePositon);
		agent->SetPos(basePositon);
		agent->SetBasePosition(basePositon);
	}
}

void SpawnPoint::ManageAgentRespawning()
{
	bool isLastRespawnEnded = m_curRespawnIndex == -1;
	if (!m_respawnIndexQueue.empty() && isLastRespawnEnded)
	{
		m_curRespawnIndex = m_respawnIndexQueue.front();
		m_respawnIndexQueue.pop();
		Utils::StartTimerWithCallback(this, CC_CALLBACK_0(SpawnPoint::RespawnAgent, this),
			m_config.GetSpawnDelay());
	}
}

void SpawnPoint::SpawnAllAgents()
{
	for (int spawnedCount = 0; spawnedCount < m_config.GetSpawnCount(); spawnedCount++)
	{
		SpawnAgent();
	}
}

void SpawnPoint::RespawnAgent()
{	
	if (m_curRespawnIndex != -1)
	{
		AIAgent* agent = m_spawnedAgents[m_curRespawnIndex];
		if (agent != nullptr)
		{
			agent->Reset();
			SetAgentEnabled(m_curRespawnIndex, true);
			m_curRespawnIndex = -1;
		}		
	}	
}

void SpawnPoint::SetAgentEnabled(int agentIndex, bool isEnabled)
{
	m_spawnedAgents[agentIndex]->setVisible(isEnabled);
	m_spawnedAgents[agentIndex]->getPhysicsBody()->setEnabled(isEnabled);
}

void SpawnPoint::OnAgentDisappeared(cc::EventCustom* customEvent)
{
	if (customEvent != nullptr)
	{
		auto data = static_cast<BaseEventData*>(customEvent->getUserData());
		if(data != nullptr)
		{
			int index = FindAgentIndexById(data->GetSenderId());
			if (index != -1)
			{				
				DespawnAgent(index);
			}
		}		
	}	
}