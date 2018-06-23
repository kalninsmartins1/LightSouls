#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class AIAgent;

class SpawnPoint : public cocos2d::Node
{
public:
	static SpawnPoint* Create(const Vector2& position,  const String& agentType,
								int spawnCount, float spawnDelay, int rowPlacementCount);

	void	Update(float deltaTime);
	void	SpawnAllAgents();

private:
	SpawnPoint(const Vector2& position, const String& agentType,
		int spawnCount, float spawnDelay, int rowPlacementCount);

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
	String					m_agentType;
	int						m_spawnCount;
	float					m_spawnDelay;
	int						m_rowPlacementCount;
	int						m_curRowCount;
	Vector2					m_lastAgentPos;
	int						m_curRespawnIndex;
};

NS_LIGHTSOULS_END