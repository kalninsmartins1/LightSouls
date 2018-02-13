#pragma once

#include "cocos2d.h"

struct PhysicsContactInformation;
typedef cocos2d::PhysicsBody PhysicsBody;
typedef std::function<void(const PhysicsBody* otherBody)> ContactCallback;

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
	static PhysicsManager* getInstance();

	bool init(cocos2d::Node* pContext);
	
	// Register for contact events
	void addContactListener(const std::string& bodyName, ContactCallback onContactBegin);

private:

	// Physics world callabck for when two objects begin colliding
	bool onContactBegin(cocos2d::PhysicsContact& contact);

	std::vector<PhysicsContactListener> m_contactListeners;
};