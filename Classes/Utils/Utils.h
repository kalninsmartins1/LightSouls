#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/GameController/GameControllerInput.h"

typedef cocos2d::EventMouse::MouseButton MouseButtonCode;
typedef cocos2d::EventKeyboard::KeyCode KeyCode;

class Utils
{	
public:

	static long long getTimeStampInMilliseconds();
	static float convertMillisecondsToSeconds(long long milliseconds);
	static void startTimerWithCallback(cocos2d::Node* pNode,
		std::function<void()> callback, float time);

	static float getRandAngleInRadians();
	static cocos2d::Vec2 GetScreenFillScale(const cocos2d::Size& curSize);
	static void logVec2(const cocos2d::Vec2& v);
	static void logVec3(const cocos2d::Vec3& v);
	static void assertWithStrFormat(bool condition, const std::string& msg, const std::string& param);

	static MouseButtonCode convertStringToMouseButtonCode(const std::string& mouseButtonStr);
	static KeyCode convertStringToKeyCode(const std::string& keyCodeStr);
	static X360Axes convertStringToGameControllerAxis(const std::string& controllerAxisStr);
	static X360Button convertStringToGameControllerButton(const std::string& controllerButtonStr);

	// Returns -1 if value is negative, else returns 1
	static int getSign(const float& value);

	template <typename T, typename K>
	static bool containsKey(const std::map<T, K>& map, const T& key);

	template <typename T, typename K>
	static bool containsKey(const cocos2d::Map<T, K>& map, const T& key);

	template <typename T>
	static void clampValue(T& value, const T& minValue, const T& maxValue);
	
	template <typename T>
	static void wrapValue(T& value, const T& startValue, const T& endValue);

private:
	Utils();
};

template <typename T, typename K>
bool Utils::containsKey(const std::map<T, K>& map, const T& key)
{
	bool bDoesContainKey = true;
	auto it = map.find(key);
	if(it == map.end())
	{
		bDoesContainKey = false;
	}

	return bDoesContainKey;
}

template <typename T, typename K>
bool Utils::containsKey(const cocos2d::Map<T, K>& map, const T& key)
{
	bool bDoesContainKey = true;
	auto it = map.find(key);
	if (it == map.end())
	{
		bDoesContainKey = false;
	}

	return bDoesContainKey;
}

template <typename T>
void Utils::clampValue(T& value, const T& minValue, const T& maxValue)
{
	if(value > maxValue)
	{
		value = maxValue;
	}
	else if(value < minValue)
	{
		value = minValue;
	}	
}

template <typename T>
void Utils::wrapValue(T& value, const T& startValue, const T& endValue)
{
	if(value > endValue)
	{
		value = startValue;
	}
	else if(value < startValue)
	{
		value = endValue;
	}
}
