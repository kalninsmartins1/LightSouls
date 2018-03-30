#pragma once

#include "cocos2d.h"
#include "Input/InputTypes/GameController/GameControllerInput.h"

using MouseButtonCode = cocos2d::EventMouse::MouseButton;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

class Utils
{	
public:
	// Returns -1 if value is negative, else returns 1
	static int					GetSign(const float& value);
	static float				GetRandValueWithinRange(int minValue, int maxValue);
	static cocos2d::Vec2		GetRandomPositionWithinCircle(const cocos2d::Vec2 centerPos, float radius);
	static float				GetRandAngleInRadians();
	static cocos2d::Vec2		GetScreenFillScale(const cocos2d::Size& curSize);
	static const cocos2d::Size& GetScreenSize();

	static float		SafeDevide(const float& up, const float& down);
	static long long	GetTimeStampInMilliseconds();
	static float		ConvertMillisecondsToSeconds(long long milliseconds);
	static void			StartTimerWithCallback(cocos2d::Node* node,
							std::function<void()> callback, float time);

	static void LogVec2(const cocos2d::Vec2& v);
	static void LogVec3(const cocos2d::Vec3& v);
	static void AssertWithStrFormat(bool condition, const std::string& msg, const std::string& param);

	static MouseButtonCode	ConvertStringToMouseButtonCode(const std::string& mouseButtonStr);
	static KeyCode			ConvertStringToKeyCode(const std::string& keyCodeStr);
	static X360Axes			ConvertStringToGameControllerAxis(const std::string& controllerAxisStr);
	static X360Button		ConvertStringToGameControllerButton(const std::string& controllerButtonStr);

	template <typename T, typename K>
	static bool ContainsKey(const std::map<T, K>& map, const T& key);

	template <typename T, typename K>
	static bool ContainsKey(const cocos2d::Map<T, K>& map, const T& key);

	template <typename T>
	static void ClampValue(T& value, const T& minValue, const T& maxValue);
	
	template <typename T>
	static void WrapValue(T& value, const T& startValue, const T& endValue);

private:
	Utils();
};

template <typename T, typename K>
bool Utils::ContainsKey(const std::map<T, K>& map, const T& key)
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
bool Utils::ContainsKey(const cocos2d::Map<T, K>& map, const T& key)
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
void Utils::ClampValue(T& value, const T& minValue, const T& maxValue)
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
void Utils::WrapValue(T& value, const T& startValue, const T& endValue)
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
