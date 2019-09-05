#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/String/String.h"

class Vector2;
class PhysicsBodyConfig;
struct PhysicsContactInformation;

using ContactCallback = std::function<bool(const Vector2& contactPoint, const cc::PhysicsBody* otherBody)>;
using QueryRectCallback = cc::PhysicsQueryRectCallbackFunc;
using RaycastCallback = cc::PhysicsRayCastCallbackFunc;

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
	static const String&	GetEventOnCollisionBegin();
	static const String&	GetEventOnPhysicsBodyAnchorSet();
	
	static PhysicsManager*	Create(cc::Node* context);
	bool					Init(cocos2d::Node* context);
	void					RemoveContactBeginListener(const String& bodyName);
	

	// Register for begin contact events
	void AddContactBeginListener(const String& bodyName, ContactCallback onContactBegin);

	// Register for end contact events
	void AddContactEndListener(const String& bodyName, ContactCallback onContactEnd);

	// Add physics body as component to given attachment node
	static void AddPhysicsBody(cocos2d::Node& attachmentNode,
		const PhysicsBodyConfig& bodyConfig);	

	void OnReload();
	void DebugDrawRect(const cocos2d::Rect& rect);

	// Checks for overlapping entities in queried square area
	static void QuerryRect(const cocos2d::Rect& rect, const QueryRectCallback& callback);
	static void Raycast(RaycastCallback callback, const Vector2& startPoint, const Vector2& endPoint, void* data = nullptr);

private:
	// Return true if all the listeners agree to collide
	bool DispatchContactEventsToListeners(const Vector2& contactPoint, const cocos2d::PhysicsBody* bodyA, const cocos2d::PhysicsBody* bodyB, const std::vector<PhysicsContactListener>& listeners);

	// Physics world callback for when two objects begin colliding
	bool OnContactBegin(cc::PhysicsContact& contact);

	// Physics world callback for when two objects stop colliding
	bool OnContactEnd(cc::PhysicsContact& contact);
	void InitDebugDraw();

private:
	static const String					s_onCollisionBeginEvent;
	static const String					s_onPhysicsBodyAnchorSet;
	std::vector<PhysicsContactListener> m_beginContactListeners;
	std::vector<PhysicsContactListener> m_endContactListeners;

	cc::Node*		m_context;
	cc::DrawNode*	m_debugDrawNode;
};