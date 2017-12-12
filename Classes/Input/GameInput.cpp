#include "GameInput.h"

using namespace cocos2d;

GameInput::GameInput()
{

}

GameInput* GameInput::getInstance()
{
	static GameInput instance;
	return &instance;
}

Vec2 GameInput::getInputAxis() const
{
	return Vec2::ZERO;
}

bool GameInput::getButtonDown() const
{
	return true;
}

bool GameInput::getKeyDown() const
{
	return true;
}

bool GameInput::getButtonPress() const
{
	return true;
}
