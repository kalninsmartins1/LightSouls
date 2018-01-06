#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/GameController/GameControllerInput.h"

typedef cocos2d::EventMouse::MouseButton MouseButtonCode;
typedef cocos2d::EventKeyboard::KeyCode KeyCode;

class Utils
{	
public:
	static bool isStrEqual(const std::string& a, const std::string& b);
	static void startTimerWithCallback(cocos2d::Node* pNode,
		std::function<void()> callback, float time);
	
	static void logVec2(const cocos2d::Vec2& v);
	static void assertWithStrFormat(bool condition, const char* msg, const char* param);

	static MouseButtonCode convertStringToMouseButtonCode(const char* mouseButtonStr);
	static KeyCode convertStringToKeyCode(const char* keyCodeStr);
	static X360Axes convertStringToGameControllerAxis(const char* controllerAxisStr);
	static X360Button convertStringToGameControllerButton(const char* controllerButtonStr);

	// Returns -1 if value is negative, else returns 1
	static int getSign(const float& value);

	template <typename T, typename K>
	static bool containsKey(const std::map<T, K>& map, const T& key);

	template <typename T>
	static void clampValue(T& value, const T& minValue, const T& maxValue);

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
