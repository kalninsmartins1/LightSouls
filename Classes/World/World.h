#pragma once

#include "cocos2d.h"

class World : public cocos2d::Sprite
{
public:
	static World*	Create(const std::string& pathToXML);	

	bool Init(const std::string& pathToSprite, const std::string& bodyName, const std::string& pathToCollisionData);

private:
	bool Init(const std::string& pathToXML);
};
