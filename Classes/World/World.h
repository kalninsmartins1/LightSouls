#pragma once

#include "LightSoulsTypes.h"

class World : public cocos2d::Sprite
{
public:
	World();

public:
	static const String& GetNodeName();
	
	static World*	Create(const String& pathToXML);
	bool			Init(const String& pathToSprite);

private:
	bool InitWithXML(const String& pathToXML);
	void OnPhysicsBodyAchorSetEvent(cc::EventCustom* eventData);

private:
	static const String s_nodeName;	
	Vector2				m_physicsBodyAnchor;
};

