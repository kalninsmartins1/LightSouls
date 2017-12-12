#pragma once

#include "cocos2d.h"

class Character;

class CameraController: cocos2d::Ref
{
public:
	CREATE_FUNC(CameraController);

	bool init();
	void moveCameraTo(cocos2d::Vec2 worldPosition, float movementSpeed);

};