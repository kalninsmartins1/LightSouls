#pragma once

#include "cocos2d.h"

struct PhysicsContactInformation;
class PhysicsBodyConfig;

using PhysicsBody = cocos2d::PhysicsBody;
using ContactCallback = std::function<void(const PhysicsBody* otherBody)>;

struct PhysicsContactListener
{
	std::string name;
	ContactCallback onContactCallback;

	PhysicsContactListener(const std::string& name, ContactCallback onContactCallback)
	{
		this->name = name;
		this->onContactCallback = onContactCallback;
	}
};

class PhysicsManager
{
public:
	using Vector2 = cocos2d::Vec2;

public:
	static PhysicsManager* GetInstance();
	
	bool Init(cocos2d::Node* context);	
	
	// Register for contact events
	void AddContactListener(const std::string& bodyName, ContactCallback onContactBegin);

	// Add physics body as component to given attachment node
	static void AddPhysicsBody(cocos2d::Node& attachmentNode,
		const PhysicsBodyConfig& bodyConfig);

	void DebugDrawRect(const cocos2d::Rect& rect);

	// Checks for overlapping entities in querried circle area
	static void QuerryRect(const cocos2d::Rect& rect,
		const cocos2d::PhysicsQueryRectCallbackFunc& callback);

private:

	// Physics world callback for when two objects begin colliding
	bool OnContactBegin(cocos2d::PhysicsContact& contact);

private:
	std::vector<PhysicsContactListener> m_contactListeners;
	cocos2d::Node* m_context;
	cocos2d::DrawNode* m_debugDrawNode;
};