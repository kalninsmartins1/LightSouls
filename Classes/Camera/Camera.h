#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class Camera : public cocos2d::Camera
{
public:
	static Camera* Create(const String& pathToXML);

private:
	bool Init(const String& pathToXML);
};

NS_LIGHTSOULS_END