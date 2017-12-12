#pragma once

#include "math/Vec2.h"

class GameInput
{
public:
	static GameInput* getInstance();
	cocos2d::Vec2 getInputAxis() const;
	bool getButtonDown() const;
	bool getKeyDown() const;
	bool getButtonPress() const;

private:
	GameInput();

	cocos2d::Vec2 m_inputAxis;
};
