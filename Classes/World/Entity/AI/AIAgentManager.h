#pragma once

#include "AIAgent.h"



class SpawnPoint;
class SpawnPointConfig;

class AIAgentManager
{
public:
	static AIAgentManager*  GetInstance();
	Entity*					GetTargetEntity() const;
	void					GetPathToAgentType(const String& type, String& outPath) const;
		
	void SetTargetEntity(Entity* entity);
	void SetTimeModifier(float timeModifier);
	
	bool Init(cocos2d::Node* worldLayer, const String& pathToXML);
	void Update(float deltaTime);
	void AddAgentConfig(const String& type, const String& configPath);
	void AddSpawnPoint(const SpawnPointConfig& config);
	void Cleanup();

private:
	AIAgentManager();

private:
	cocos2d::Node*			 m_worldLayer;
	Entity*					 m_targetEntity;
	std::vector<SpawnPoint*> m_allActiveSpawnPoints;
	std::map<String, String> m_agentConfigPathMap;
};


