#pragma once
#include "cocos2d.h"

class World : public cocos2d::Sprite
{
public:
	static World* create(const char* pathToXML);

private:
	bool init(const char* pathToXML);
};
