#pragma once

#include "cocos2d.h"

class Character;

class CameraController
{
public:
	bool init();
	void moveCameraTo(cocos2d::Vec2 worldPosition, float movementSpeed);

private:
};