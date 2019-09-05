#pragma once

#include "Classes/External/CocosEngine.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

class World : public cc::Sprite
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

