#include "Utils.h"
#include "GameConsts.h"
#include "Input/InputTypes/Keyboard/KeyboardInput.h"

using namespace cocos2d;

Utils::Utils()
{

}

bool Utils::isStrEqual(const std::string& a, const std::string& b)
{
	return a.compare(b) == 0;
}

void Utils::logVec2(const cocos2d::Vec2& v)
{
	cocos2d::log("(X: %f, Y: %f)", v.x, v.y);
}

void Utils::startTimerWithCallback(Node* pNode, std::function<void()> callback,
	float time)
{
	Vector<FiniteTimeAction*> callbackTimerActions;
	callbackTimerActions.pushBack(DelayTime::create(time));
	callbackTimerActions.pushBack(CallFunc::create(callback));
	
	// Start timer with callback
	pNode->runAction(Sequence::create(callbackTimerActions));
}

MouseButtonCode Utils::convertStringToMouseButtonCode(const char* string)
{
	std::string standartString(string);
	MouseButtonCode result = MouseButtonCode::BUTTON_UNSET;

	if(isStrEqual(standartString, "LeftButton"))
	{
		result = MouseButtonCode::BUTTON_LEFT;
	}
	else if(isStrEqual(standartString, "RightButton"))
	{
		result = MouseButtonCode::BUTTON_RIGHT;
	}

	return result;
}

KeyCode Utils::convertStringToKeyCode(const char* string)
{
	std::string standartString(string);
	KeyCode result = KeyCode();

	if (isStrEqual(standartString, "W"))
	{
		result = KeyCode::KEY_W;
	}
	else if (isStrEqual(standartString, "S"))
	{
		result = KeyCode::KEY_S;
	}
	else if (isStrEqual(standartString, "A"))
	{
		result = KeyCode::KEY_A;
	}
	else if(isStrEqual(standartString, "D"))
	{
		result = KeyCode::KEY_D;
	}
	else if(isStrEqual(standartString, "Space"))
	{
		result = KeyCode::KEY_SPACE;
	}

	return result;
}

int Utils::getSign(const float & value)
{
	int sign = 1;
	if (value < 0)
	{
		sign = -1;
	}
	return sign;
}
