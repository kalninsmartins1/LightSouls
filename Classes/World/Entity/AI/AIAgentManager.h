#pragma once

#include "base/CCRef.h"
#include "AIAgent.h"

class AIAgentManager: cocos2d::Ref
{
public:
	static AIAgentManager* getInstance();
	
	bool init(const std::string& pathToXML);

	void setWorldLayer(cocos2d::Node* pWorldLayer);
	void setTargetEntity(const Entity* pEntity);

	const Entity& getTargetEntity() const;
	
	void update(float deltaTime);
	void addAgent(const std::string& type, AIAgent* pAgent);
	void spawnAgent(const std::string& type, const cocos2d::Vec2& position);

private:	
	AIAgentManager();

	cocos2d::Node* m_pWorldLayer;
	const Entity* m_pTargetEntity;

	// Use cocos2d Map to retain pointers
	cocos2d::Map<std::string, AIAgent*> m_agentPrefabMap;
	std::vector<AIAgent*> m_allActiveAgents;
};
