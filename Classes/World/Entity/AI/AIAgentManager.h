#pragma once

#include "base/CCRef.h"
#include "AIAgent.h"

NS_LIGHTSOULS_BEGIN

class AIAgentManager: cocos2d::Ref
{
public:
	static AIAgentManager* GetInstance();
	
	bool Init(const String& pathToXML);

	void SetWorldLayer(cocos2d::Node* pWorldLayer);
	void SetTargetEntity(const Entity* pEntity);

	const Entity& GetTargetEntity() const;
	
	void Update(float deltaTime);
	void addAgentConfig(const String& type, const String& configPath);
	void SpawnAgent(const String& type, const Vector2& position);

private:	
	AIAgentManager();

	cocos2d::Node* m_worldLayer;
	const Entity* m_pTargetEntity;

	// Use cocos2d Map to retain pointers
	std::map<String, String> m_agentConfigPathMap;
	std::vector<AIAgent*> m_allActiveAgents;
};

NS_LIGHTSOULS_END
