#include "Utils.h"
#include "Classes/Core/Input/InputTypes/Keyboard/KeyboardInput.h"
#include "World/Entity/Components/MirrorSpriteComponent.h"
#include "World/Entity/Components/AnimComponent.h"
#include "Classes/Core/Input/InputTypes/GameController/GameControllerInput.h"
#include "Classes/Core/Input/InputTypes/Mouse/MouseInput.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

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

float Utils::GetScaleFactor()
{
	return cocos2d::Director::getInstance()->getContentScaleFactor();
}

float Utils::ConvertMillisecondsToSeconds(long long milliseconds)
{
	return milliseconds / 1000.f;
}

void Utils::LogVector2(const Vector2& v)
{
	CCLOG("(X: %f, Y: %f)", v.GetX(), v.GetY());
}

void Utils::LogVector3(const Vector3& v)
{
	CCLOG("(X: %f, Y: %f, Z: %f)", v.x, v.y, v.z);
}

void Utils::AssertWithStrFormat(bool condition, const String& msg,
	const String& param)
{
	CCASSERT(condition, cc::StringUtils::format(msg.GetCStr(), param.GetCStr()).c_str());
}

Vector2 Utils::GetScreenFillScale(const cc::Size& curSize)
{
	Vector2 scale = Vector2::GetZero();
	if(curSize.width > 0 && curSize.height > 0)
	{
		const cocos2d::Size& winSize = GetScreenSize();
		scale.SetX(winSize.width / curSize.width);
		scale.SetY(winSize.height / curSize.height);
	}
	
	return scale;
}

const cocos2d::Size& Utils::GetScreenSize()
{
	return cocos2d::Director::getInstance()->getWinSize();
}

float Utils::SafeDivide(const float& up, const float& down)
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

void Utils::StartTimerWithCallback(cocos2d::Node* node, std::function<void()> callback,
	float time, int tag)
{
	using namespace cocos2d;

	Vector<FiniteTimeAction*> callbackTimerActions;
	callbackTimerActions.pushBack(DelayTime::create(time));
	callbackTimerActions.pushBack(CallFunc::create(callback));
	
	auto sequence = Sequence::create(callbackTimerActions);
	sequence->setTag(tag);

	// Start timer with callback
	node->runAction(sequence);
}

void Utils::ClearCallbackTimers(cocos2d::Node* node, int timerTag)
{
	node->stopActionByTag(timerTag);
}

void Utils::ParseFloatArray(const String& data, std::vector<float>& outArray)
{
	unsigned int index = 0;
	String midResult = "";
	while (index < data.GetLength())
	{
		if (data[index] == ';')
		{
			outArray.push_back(atof(midResult.GetCStr()));
			midResult = "";
		}
		else
		{
			midResult += data[index];
		}

		index++;
	}
}

void Utils::ToggleMouseCursor(bool isEnabled)
{
	auto* director = cc::Director::getInstance();
	if (director != nullptr)
	{
		auto* gameView = director->getOpenGLView();
		if (gameView != nullptr)
		{
			gameView->setCursorVisible(isEnabled);
		}
	}
}

Vector2 Utils::GetRandomPositionWithinCircle(const Vector2& centerPos, float radius)
{
	// Get random position within range
	const float angle = Utils::GetRandAngleInRadians();
	const float targetX = cos(angle) * radius;
	const float targetY = sin(angle) * radius;

	// Get time it takes for agent to move to position
	const Vector2 randomPosition = centerPos +
		Vector2(targetX, targetY);

	return randomPosition;
}

Vector2 Utils::GetRandomPositionWithinCircle(const Vector3& centerPos, float radius)
{	
	return GetRandomPositionWithinCircle(Vector2(centerPos.x, centerPos.y), radius);
}

int Utils::GetRandValueWithinRange(int minValue, int maxValue)
{
	int range = maxValue - minValue;
	return minValue + (rand() % range);
}

float Utils::GetRandValueWithinRange(float minValue, int maxValue)
{
	float range = maxValue - minValue;
	int rangeScaled = static_cast<int>(range * 1000.0f);
	float randValue = (rand() % rangeScaled) / 1000.0f;

	return minValue + randValue;
}

float Utils::GetRandAngleInRadians()
{
	return (rand() % GameConsts::FULL_CIRCLE_DEGREES) * (GameConsts::PI / GameConsts::HALF_CIRCLE_DEGREES);
}

float Utils::GetSignedAngleBetweenVectors(const Vector2& v1, const Vector2& v2)
{	
	return CC_RADIANS_TO_DEGREES(GetSignedRadiansBetweenVectors(v1, v2));
}

float Utils::GetSignedRadiansBetweenVectors(const Vector2& v1, const Vector2& v2)
{
	float result = atan2(v2.GetY(), v2.GetX()) - atan2(v1.GetY(), v1.GetX());
	if (result < 0)
	{
		result += 2 * GameConsts::PI;
	}
	return result;
}

MouseAxisType Utils::ConvertStringToMouseAxisType(const String & axisName)
{
	MouseAxisType result = MouseAxisType::NONE;

	if (axisName == "X")
	{
		result = MouseAxisType::X;
	}
	else if (axisName == "Y")
	{
		result = MouseAxisType::Y;
	}

	return result;
}

MouseButtonCode Utils::ConvertStringToMouseButtonCode(const String& mouseButtonStr)
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

KeyCode Utils::ConvertStringToKeyCode(const String& keyCodeStr)
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
	else if (keyCodeStr == "Esc")
	{
		result = KeyCode::KEY_ESCAPE;
	}
	else if (keyCodeStr == "F2")
	{
		result = KeyCode::KEY_F2;
	}
	else if (keyCodeStr == "F3")
	{
		result = KeyCode::KEY_F3;
	}
	else
	{
		AssertWithStrFormat(false, "Utils: Invalid key code keyCodeStr %s", keyCodeStr);
	}

	return result;
}

X360Axes Utils::ConvertStringToGameControllerAxis(const String& controllerAxisStr)
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

X360Button Utils::ConvertStringToGameControllerButton(const String& controllerButtonStr)
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


