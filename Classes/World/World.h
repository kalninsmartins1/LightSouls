#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class World : public cocos2d::Sprite
{
public:
	static const String& GetNodeName();
	
	static World*	Create(const String& pathToXML);
	bool			Init(const String& pathToSprite, const String& bodyName, const String& pathToCollisionData);

private:
	bool Init(const String& pathToXML);

private:
	static const String s_nodeName;
	static bool			s_isShapeCacheInitialized;
};

NS_LIGHTSOULS_END