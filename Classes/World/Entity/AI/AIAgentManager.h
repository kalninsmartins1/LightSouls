#pragma once

#include "AIAgent.h"

NS_LIGHTSOULS_BEGIN

class AIAgentManager: cocos2d::Ref
{
public:
	static AIAgentManager*  GetInstance();
	const Entity&			GetTargetEntity() const;
	
	void SetWorldLayer(cocos2d::Node* pWorldLayer);
	void SetTargetEntity(const Entity* pEntity);
	
	bool Init(const String& pathToXML);
	void Update(float deltaTime);
	void addAgentConfig(const String& type, const String& configPath);
	void SpawnAgent(const String& type, const Vector2& position);
	void DespwanAgent(unsigned int agentIndex);

private:
	AIAgentManager();

private:
	cocos2d::Node* m_worldLayer;
	const Entity*  m_targetEntity;

	// Use cocos2d Map to retain pointers
	std::map<String, String> m_agentConfigPathMap;
	std::vector<AIAgent*>	 m_allActiveAgents;	
};

NS_LIGHTSOULS_END
