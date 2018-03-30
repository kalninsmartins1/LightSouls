#include "Utils.h"
#include "Input/InputTypes/Keyboard/KeyboardInput.h"
#include "GameConsts.h"
#include "World/Entity/Components/MirrorSpriteComponent.h"
#include "World/Entity/Components/AIAnimComponent.h"

using namespace cocos2d;

Utils::Utils()
{
	// Private constructor
}

long long Utils::GetTimeStampInMilliseconds()
{
	// Convert system ticks to milliseconds	
	const long long ticks = std::chrono::system_clock::now().time_since_epoch().count();
	return ticks / 10000;
}

float Utils::ConvertMillisecondsToSeconds(long long milliseconds)
{
	return milliseconds / 1000.f;
}

void Utils::LogVec2(const Vec2& v)
{
	CCLOG("(X: %f, Y: %f)", v.x, v.y);
}

void Utils::LogVec3(const Vec3& v)
{
	CCLOG("(X: %f, Y: %f, Z: %f)", v.x, v.y, v.z);
}

void Utils::AssertWithStrFormat(bool condition, const std::string& msg,
	const std::string& param)
{
	CCASSERT(condition, StringUtils::format(msg.c_str(), param).c_str());
}

Vec2 Utils::GetScreenFillScale(const Size& curSize)
{
	Vec2 scale = Vec2::ZERO;
	if(curSize.width > 0 && curSize.height > 0)
	{
		const Size& winSize = GetScreenSize();
		scale.x = winSize.width / curSize.width;
		scale.y = winSize.height / curSize.height;
	}
	
	return scale;
}

const cocos2d::Size& Utils::GetScreenSize()
{
	return Director::getInstance()->getWinSize();
}

float Utils::SafeDevide(const float& up, const float& down)
{
	float result = 0;
	if (down > 0)
	{
		result = up/down;
	}
	else
	{
		CCASSERT(false, "Error trying to devide with 0 !");
	}
	
	return result;
}

void Utils::StartTimerWithCallback(Node* pNode, std::function<void()> callback,
	float time)
{
	Vector<FiniteTimeAction*> callbackTimerActions;
	callbackTimerActions.pushBack(DelayTime::create(time));
	callbackTimerActions.pushBack(CallFunc::create(callback));
	
	// Start timer with callback
	pNode->runAction(Sequence::create(callbackTimerActions));
}

cocos2d::Vec2 Utils::GetRandomPositionWithinCircle(const cocos2d::Vec2 centerPos, float radius)
{
	// Get random position within range
	const float angle = Utils::GetRandAngleInRadians();
	const float targetX = cos(angle) * radius;
	const float targetY = sin(angle) * radius;

	// Get time it takes for agent to move to position
	const Vec2 randomPosition = centerPos +
		Vec2(targetX, targetY);

	return randomPosition;
}

float Utils::GetRandValueWithinRange(int minValue, int maxValue)
{
	int range = maxValue - minValue;
	return minValue + (rand() % range);
}

float Utils::GetRandAngleInRadians()
{
	return (rand() % FULL_CIRCLE_DEGREES) * (PI / HALF_CIRCLE_DEGREES);
}

MouseButtonCode Utils::ConvertStringToMouseButtonCode(const std::string& mouseButtonStr)
{	
	MouseButtonCode result = MouseButtonCode::BUTTON_UNSET;

	if(mouseButtonStr == "LeftButton")
	{
		result = MouseButtonCode::BUTTON_LEFT;
	}
	else if(mouseButtonStr == "RightButton")
	{
		result = MouseButtonCode::BUTTON_RIGHT;
	}
	else
	{
		AssertWithStrFormat(false, "Utils: Invalid button code %s !", mouseButtonStr);
	}

	return result;
}

KeyCode Utils::ConvertStringToKeyCode(const std::string& keyCodeStr)
{	
	KeyCode result = KeyCode::KEY_NONE;

	if (keyCodeStr == "W")
	{
		result = KeyCode::KEY_W;
	}
	else if (keyCodeStr == "S")
	{
		result = KeyCode::KEY_S;
	}
	else if (keyCodeStr == "A")
	{
		result = KeyCode::KEY_A;
	}
	else if(keyCodeStr == "D")
	{
		result = KeyCode::KEY_D;
	}
	else if(keyCodeStr == "Space")
	{
		result = KeyCode::KEY_SPACE;
	}
	else
	{
		AssertWithStrFormat(false, "Utils: Invalid key code keyCodeStr %s", keyCodeStr);
	}

	return result;
}

X360Axes Utils::ConvertStringToGameControllerAxis(const std::string& controllerAxisStr)
{
	X360Axes result = X360Axes::NONE;
	if(controllerAxisStr == "LEFT_STICK_X")
	{
		result = X360Axes::LEFT_STICK_X;
	}
	else if(controllerAxisStr == "LEFT_STICK_Y")
	{
		result = X360Axes::LEFT_STICK_Y;
	}
	else if (controllerAxisStr == "RIGHT_STICK_X")
	{
		result = X360Axes::RIGHT_STICK_X;
	}
	else if (controllerAxisStr == "RIGHT_STICK_Y")
	{
		result = X360Axes::RIGHT_STICK_Y;
	}
	else if (controllerAxisStr == "LT")
	{
		result = X360Axes::LT;
	}
	else if (controllerAxisStr == "RT")
	{
		result = X360Axes::RT;
	}

	return result;
}

X360Button Utils::ConvertStringToGameControllerButton(const std::string& controllerButtonStr)
{
	X360Button result = X360Button::NONE;
	if (controllerButtonStr == "A")
	{
		result = X360Button::A;
	}
	else if(controllerButtonStr == "B")
	{
		result = X360Button::B;
	}
	else if(controllerButtonStr == "X")
	{
		result = X360Button::X;
	}
	else if (controllerButtonStr == "Y")
	{
		result = X360Button::Y;
	}
	else if(controllerButtonStr == "LB")
	{
		result = X360Button::LB;
	}
	else if(controllerButtonStr == "RB")
	{
		result = X360Button::RB;
	}
	else if (controllerButtonStr == "BACK")
	{
		result = X360Button::BACK;
	}
	else if (controllerButtonStr == "START")
	{
		result = X360Button::START;
	}
	else if (controllerButtonStr == "LEFT_STICK")
	{
		result = X360Button::LEFT_STICK;
	}
	else if (controllerButtonStr == "RIGHT_STICK")
	{
		result = X360Button::RIGHT_STICK;
	}
	else if (controllerButtonStr == "UP")
	{
		result = X360Button::UP;
	}
	else if (controllerButtonStr == "RIGHT")
	{
		result = X360Button::RIGHT;
	}
	else if (controllerButtonStr == "DOWN")
	{
		result = X360Button::DOWN;
	}
	else if (controllerButtonStr == "LEFT")
	{
		result = X360Button::LEFT;
	}

	return result;
}

int Utils::GetSign(const float & value)
{
	int sign = 1;
	if (value < 0)
	{
		sign = -1;
	}
	return sign;
}
