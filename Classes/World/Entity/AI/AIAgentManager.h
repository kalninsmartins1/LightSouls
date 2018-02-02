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
	void addAgentConfig(const std::string& type, const std::string& configPath);
	void spawnAgent(const std::string& type, const cocos2d::Vec2& position);

private:	
	AIAgentManager();

	cocos2d::Node* m_pWorldLayer;
	const Entity* m_pTargetEntity;

	// Use cocos2d Map to retain pointers
	std::map<std::string, std::string> m_agentConfigPathMap;
	std::vector<AIAgent*> m_allActiveAgents;
};
