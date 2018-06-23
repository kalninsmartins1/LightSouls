#pragma once

#include "AIAgent.h"

NS_LIGHTSOULS_BEGIN

class SpawnPoint;

class AIAgentManager
{
public:
	static AIAgentManager*  GetInstance();
	const Entity&			GetTargetEntity() const;
	void					GetPathToAgentType(const String& type, String& outPath) const;
		
	void SetTargetEntity(const Entity* entity);
	
	bool Init(cocos2d::Node* worldLayer, const String& pathToXML);
	void Update(float deltaTime);
	void AddAgentConfig(const String& type, const String& configPath);
	void AddSpawnPoint(const Vector2& position, const String& agentType, int spawnCount, float spawnDelay, int rowPlacementCount);

private:
	AIAgentManager();

private:
	cocos2d::Node*			 m_worldLayer;
	const Entity*			 m_targetEntity;
	std::vector<SpawnPoint*> m_allActiveSpawnPoints;
	std::map<String, String> m_agentConfigPathMap;
};

NS_LIGHTSOULS_END
