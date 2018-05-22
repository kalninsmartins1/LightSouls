#pragma once

#include "LightSoulsTypes.h"
#include "GameConsts.h"

NS_LIGHTSOULS_BEGIN

enum class X360Axes;
enum class X360Button;

using MouseButtonCode = cocos2d::EventMouse::MouseButton;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

class Utils
{	
public:
	// Returns -1 if value is negative, else returns 1
	static int					GetSign(const float& value);
	static float				GetRandValueWithinRange(int minValue, int maxValue);
	static Vector2				GetRandomPositionWithinCircle(const Vector2& centerPos, float radius);
	static Vector2				GetRandomPositionWithinCircle(const Vector3& centerPos, float radius);
	static float				GetRandAngleInRadians();
	static Vector2				GetScreenFillScale(const cocos2d::Size& curSize);
	static const cocos2d::Size& GetScreenSize();
	static long long			GetTimeStampInMilliseconds();
	static float				GetScaleFactor();

	static float		SafeDevide(const float& up, const float& down);
	static float		ConvertMillisecondsToSeconds(long long milliseconds);
	static void			StartTimerWithCallback(cocos2d::Node* node,
							std::function<void()> callback, float time, int tag = ACTION_TIMER_TAG);

	static void LogVector2(const Vector2& v);
	static void LogVector3(const Vector3& v);
	static void AssertWithStrFormat(bool condition, const String& msg, const String& param);

	static MouseButtonCode	ConvertStringToMouseButtonCode(const String& mouseButtonStr);
	static KeyCode			ConvertStringToKeyCode(const String& keyCodeStr);
	static X360Axes			ConvertStringToGameControllerAxis(const String& controllerAxisStr);
	static X360Button		ConvertStringToGameControllerButton(const String& controllerButtonStr);

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
	bool doesContainKey = true;
	auto it = map.find(key);
	if(it == map.end())
	{
		doesContainKey = false;
	}

	return doesContainKey;
}

template <typename T, typename K>
bool Utils::ContainsKey(const cocos2d::Map<T, K>& map, const T& key)
{
	bool doesContainKey = true;
	auto it = map.find(key);
	if (it == map.end())
	{
		doesContainKey = false;
	}

	return doesContainKey;
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

NS_LIGHTSOULS_END