#pragma once

#include "LightSoulsTypes.h"

struct PhysicsContactInformation;
class PhysicsBodyConfig;

using ContactCallback = std::function<bool(const Vector2& contactPoint, const cocos2d::PhysicsBody* otherBody)>;
using QueryRectCallback = cocos2d::PhysicsQueryRectCallbackFunc;
using RaycastCallback = cocos2d::PhysicsRayCastCallbackFunc;

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

class PhysicsManager : public cocos2d::Ref
{
public:
	~PhysicsManager();

public:
	static const String&	GetEventOnCollisionBegin();
	
	static PhysicsManager*	Create(cocos2d::Node* context);
	bool					Init(cocos2d::Node* context);
	void					RemoveContactBeginListener(const String& bodyName);
	

	// Register for begin contact events
	void AddContactBeginListener(const String& bodyName, ContactCallback onContactBegin);

	// Register for end contact events
	void AddContactEndListener(const String& bodyName, ContactCallback onContactEnd);

	// Add physics body as component to given attachment node
	static void AddPhysicsBody(cocos2d::Node& attachmentNode,
		const PhysicsBodyConfig& bodyConfig);

	void DebugDrawRect(const cocos2d::Rect& rect);

	// Checks for overlapping entities in queried square area
	static void QuerryRect(const cocos2d::Rect& rect, const QueryRectCallback& callback);
	static void Raycast(RaycastCallback callback, const Vector2& startPoint, const Vector2& endPoint, void* data = nullptr);

private:
	// Return true if all the listeners agree to collide
	bool DispatchContactEventsToListeners(const Vector2& contactPoint, const cocos2d::PhysicsBody* bodyA, const cocos2d::PhysicsBody* bodyB, const std::vector<PhysicsContactListener>& listeners);

	// Physics world callback for when two objects begin colliding
	bool OnContactBegin(cocos2d::PhysicsContact& contact);

	// Physics world callback for when two objects stop colliding
	bool OnContactEnd(cocos2d::PhysicsContact& contact);

private:
	static const String					s_onCollisionBeginEvent;
	std::vector<PhysicsContactListener> m_beginContactListeners;
	std::vector<PhysicsContactListener> m_endContactListeners;

	cocos2d::Node*		m_context;
	cocos2d::DrawNode*	m_debugDrawNode;
};