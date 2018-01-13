#include "Utils.h"
#include "Input/InputTypes/Keyboard/KeyboardInput.h"

using namespace cocos2d;

Utils::Utils()
{
	// Private constructor
}

long Utils::getTimeStampInMilliseconds()
{
	// Convert system ticks to milliseconds
	return std::chrono::system_clock::now().time_since_epoch().count() / 10000;
}

bool Utils::isStrEqual(const std::string& a, const std::string& b)
{
	// TODO: Do we really need a function for this ?
	return a == b;
}

void Utils::logVec2(const cocos2d::Vec2& v)
{
	CCLOG("(X: %f, Y: %f)", v.x, v.y);
}

void Utils::logVec3(const cocos2d::Vec3& v)
{
	CCLOG("(X: %f, Y: %f, Z: %f)", v.x, v.y, v.z);
}

void Utils::assertWithStrFormat(bool condition, const char* msg, const char* param)
{
	CCASSERT(condition, StringUtils::format(msg, param).c_str());
}

Vec2 Utils::getScreenFillScale(const Size& curSize)
{
	Vec2 scale = Vec2::ZERO;
	if(curSize.width > 0 && curSize.height > 0)
	{
		const Size& winSize = Director::getInstance()->getWinSize();
		scale.x = winSize.width / curSize.width;
		scale.y = winSize.height / curSize.height;
	}
	
	return scale;
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

MouseButtonCode Utils::convertStringToMouseButtonCode(const char* mouseButtonStr)
{	
	MouseButtonCode result = MouseButtonCode::BUTTON_UNSET;

	if(isStrEqual(mouseButtonStr, "LeftButton"))
	{
		result = MouseButtonCode::BUTTON_LEFT;
	}
	else if(isStrEqual(mouseButtonStr, "RightButton"))
	{
		result = MouseButtonCode::BUTTON_RIGHT;
	}
	else
	{
		assertWithStrFormat(false, "Utils: Invalid button code %s !", mouseButtonStr);
	}

	return result;
}

KeyCode Utils::convertStringToKeyCode(const char* keyCodeStr)
{	
	KeyCode result = KeyCode::KEY_NONE;

	if (isStrEqual(keyCodeStr, "W"))
	{
		result = KeyCode::KEY_W;
	}
	else if (isStrEqual(keyCodeStr, "S"))
	{
		result = KeyCode::KEY_S;
	}
	else if (isStrEqual(keyCodeStr, "A"))
	{
		result = KeyCode::KEY_A;
	}
	else if(isStrEqual(keyCodeStr, "D"))
	{
		result = KeyCode::KEY_D;
	}
	else if(isStrEqual(keyCodeStr, "Space"))
	{
		result = KeyCode::KEY_SPACE;
	}
	else
	{
		assertWithStrFormat(false, "Utils: Invalid key code keyCodeStr %s", keyCodeStr);
	}

	return result;
}

X360Axes Utils::convertStringToGameControllerAxis(const char* controllerAxisStr)
{
	X360Axes result = X360Axes::NONE;
	if(isStrEqual(controllerAxisStr, "LEFT_STICK_X"))
	{
		result = X360Axes::LEFT_STICK_X;
	}
	else if(isStrEqual(controllerAxisStr, "LEFT_STICK_Y"))
	{
		result = X360Axes::LEFT_STICK_Y;
	}
	else if (isStrEqual(controllerAxisStr, "RIGHT_STICK_X"))
	{
		result = X360Axes::RIGHT_STICK_X;
	}
	else if (isStrEqual(controllerAxisStr, "RIGHT_STICK_Y"))
	{
		result = X360Axes::RIGHT_STICK_Y;
	}
	else if (isStrEqual(controllerAxisStr, "LT"))
	{
		result = X360Axes::LT;
	}
	else if (isStrEqual(controllerAxisStr, "RT"))
	{
		result = X360Axes::RT;
	}

	return result;
}

X360Button Utils::convertStringToGameControllerButton(const char* controllerButtonStr)
{
	X360Button result = X360Button::NONE;
	if (isStrEqual(controllerButtonStr, "A"))
	{
		result = X360Button::A;
	}
	else if(isStrEqual(controllerButtonStr, "B"))
	{
		result = X360Button::B;
	}
	else if(isStrEqual(controllerButtonStr, "X"))
	{
		result = X360Button::X;
	}
	else if (isStrEqual(controllerButtonStr, "Y"))
	{
		result = X360Button::Y;
	}
	else if(isStrEqual(controllerButtonStr, "LB"))
	{
		result = X360Button::LB;
	}
	else if(isStrEqual(controllerButtonStr, "RB"))
	{
		result = X360Button::RB;
	}
	else if (isStrEqual(controllerButtonStr, "BACK"))
	{
		result = X360Button::BACK;
	}
	else if (isStrEqual(controllerButtonStr, "START"))
	{
		result = X360Button::START;
	}
	else if (isStrEqual(controllerButtonStr, "LEFT_STICK"))
	{
		result = X360Button::LEFT_STICK;
	}
	else if (isStrEqual(controllerButtonStr, "RIGHT_STICK"))
	{
		result = X360Button::RIGHT_STICK;
	}
	else if (isStrEqual(controllerButtonStr, "UP"))
	{
		result = X360Button::UP;
	}
	else if (isStrEqual(controllerButtonStr, "RIGHT"))
	{
		result = X360Button::RIGHT;
	}
	else if (isStrEqual(controllerButtonStr, "DOWN"))
	{
		result = X360Button::DOWN;
	}
	else if (isStrEqual(controllerButtonStr, "LEFT"))
	{
		result = X360Button::LEFT;
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
