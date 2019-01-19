#pragma once

#include "LightSoulsTypes.h"



class World : public cocos2d::Sprite
{
public:
	static const String& GetNodeName();
	
	static World*	Create(const String& pathToXML);
	bool			Init(const String& pathToSprite);

private:
	bool InitWithXML(const String& pathToXML);

private:
	static const String s_nodeName;
	static bool			s_isShapeCacheInitialized;
};

