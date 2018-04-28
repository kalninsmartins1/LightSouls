#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

struct PhysicsContactInformation;
class PhysicsBodyConfig;

using ContactCallback = std::function<void(const cocos2d::PhysicsBody* otherBody)>;

struct PhysicsContactListener
{
	String name;
	ContactCallback onContactCallback;

	PhysicsContactListener(const String& name, ContactCallback onContactCallback)
	{
		this->name = name;
		this->onContactCallback = onContactCallback;
	}
};

class PhysicsManager
{
public:
	static PhysicsManager* GetInstance();
	
	bool Init(cocos2d::Node* context);	
	
	// Register for begin contact events
	void AddContactBeginListener(const String& bodyName, ContactCallback onContactBegin);

	// Register for end contact events
	void AddContactEndListener(const String& bodyName, ContactCallback onContactEnd);

	// Add physics body as component to given attachment node
	static void AddPhysicsBody(cocos2d::Node& attachmentNode,
		const PhysicsBodyConfig& bodyConfig);

	void DebugDrawRect(const cocos2d::Rect& rect);

	// Checks for overlapping entities in querried circle area
	static void QuerryRect(const cocos2d::Rect& rect,
		const cocos2d::PhysicsQueryRectCallbackFunc& callback);

private:
	void DispatchContactEventsToListeners(const cocos2d::PhysicsBody* bodyA, const cocos2d::PhysicsBody* bodyB, const std::vector<PhysicsContactListener>& listeners);

	// Physics world callback for when two objects begin colliding
	bool OnContactBegin(cocos2d::PhysicsContact& contact);

	// Physics world callback for when two objects stop colliding
	bool OnContactEnd(cocos2d::PhysicsContact& contact);

private:
	std::vector<PhysicsContactListener> m_beginContactListeners;
	std::vector<PhysicsContactListener> m_endContactListeners;

	cocos2d::Node* m_context;
	cocos2d::DrawNode* m_debugDrawNode;
};

NS_LIGHTSOULS_END