#pragma once

#include "LightSoulsTypes.h"
#include "SpawnPointConfig.h"

NS_LIGHTSOULS_BEGIN

class AIAgent;

class SpawnPoint : public cocos2d::Node
{
public:
	static SpawnPoint* Create(const SpawnPointConfig& config);

	void	Update(float deltaTime);
	void	SpawnAllAgents();

private:
	SpawnPoint(const SpawnPointConfig& config);

private:
	void GetNextSpawnPosition(const cocos2d::Size& agentSize, Vector2& outPosition);

	void DespawnAgent(unsigned int agentIndex);
	void SpawnAgent(bool isRespawn = false);
	void ManageAgentRespawning();
	void RespawnAgent();
	void SetAgentEnabled(int agentIndex, bool isEnabled);

private:
	std::queue<int>			m_respawnIndexQueue;
	std::vector<AIAgent*>	m_spawnedAgents;
	SpawnPointConfig		m_config;
	int						m_curRowCount;
	Vector2					m_lastAgentPos;
	int						m_curRespawnIndex;
};

NS_LIGHTSOULS_END