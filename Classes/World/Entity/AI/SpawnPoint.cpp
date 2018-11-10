#include "SpawnPoint.h"
#include "AIAgent.h"
#include "AIAgentManager.h"
#include "Utils/Utils.h"




SpawnPoint::SpawnPoint(const SpawnPointConfig& config)
	: m_respawnIndexQueue()
	, m_spawnedAgents()
	, m_config(config)
	, m_curRowCount(0)
	, m_lastAgentPos(Vector2::ZERO)	
	, m_curRespawnIndex(-1)
{
	setPosition(config.GetPosition());
}

void SpawnPoint::GetNextSpawnPosition(const cocos2d::Size& agentSize, Vector2& outPosition)
{
	if (m_lastAgentPos == Vector2::ZERO) 
	{
		getPosition(&outPosition.x, &outPosition.y);
		m_curRowCount++;
	}
	else if(m_curRowCount < m_config.GetNumAgentsInRow())
	{
		outPosition = m_lastAgentPos + Vector2::UNIT_X * 
			(agentSize.width + m_config.GetAgentColumnPadding());
		m_curRowCount++;
	}
	else
	{
		m_curRowCount = 0;
		outPosition = Vector2(getPositionX(), m_lastAgentPos.y + 
			(agentSize.height + m_config.GetAgentRowPadding()));
	}
	m_lastAgentPos = outPosition;
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

void SpawnPoint::Update(float deltaTime)
{
	// Update all agents
	int index = 0;
	for (auto agent : m_spawnedAgents)
	{
		agent->update(deltaTime);

		// Agent went offline
		if (agent->GetHealth() <= 0 && agent->isVisible())
		{
			DespawnAgent(index);
		}
		index++;
	}

	ManageAgentRespawning();
}

void SpawnPoint::DespawnAgent(unsigned int agentIndex)
{
	SetAgentEnabled(agentIndex, false);
	m_respawnIndexQueue.push(agentIndex);
}

void SpawnPoint::SpawnAgent(bool isRespawn)
{
	String pathToXML;
	AIAgentManager::GetInstance()->GetPathToAgentType(m_config.GetAgentType(), pathToXML);

	if (!pathToXML.empty())
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
		agent->setPosition(basePositon);
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
	SetAgentEnabled(m_curRespawnIndex, true);
	m_spawnedAgents[m_curRespawnIndex]->Reset();
	m_curRespawnIndex = -1;
}

void SpawnPoint::SetAgentEnabled(int agentIndex, bool isEnabled)
{
	m_spawnedAgents[agentIndex]->setVisible(isEnabled);
	m_spawnedAgents[agentIndex]->getPhysicsBody()->setEnabled(isEnabled);
}

