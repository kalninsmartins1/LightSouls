#pragma once

#include "Classes/External/CocosEngine.h"
#include "GameConsts.h"

class Vector2;
class String;

enum class X360Axes;
enum class X360Button;
enum class MouseAxisType : char;

using Vector3 = cocos2d::Vec3;
using MouseButtonCode = cocos2d::EventMouse::MouseButton;
using KeyCode = cocos2d::EventKeyboard::KeyCode;

template <typename T>
struct Range
{
	T minVal;
	T maxVal;

	Range()
	{

	}

	Range(T minVal, T maxVal)
		: minVal(minVal)
		, maxVal(maxVal)
	{

	}
};

class Utils
{	
public:
	// Returns -1 if value is negative, else returns 1
	static int					GetSign(const float& value);

	// minValue [inclusive], maxValue [exclusive]
	static int					GetRandValueWithinRange(int minValue, int maxValue);

	// minValue [inclusive], maxValue [exclusive]
	static float				GetRandValueWithinRange(float minValue, int maxValue);

	static Vector2		GetRandomPositionWithinCircle(const Vector2& centerPos, float radius);
	static Vector2		GetRandomPositionWithinCircle(const Vector3& centerPos, float radius);
	static float				GetRandAngleInRadians();
	static float				GetSignedAngleBetweenVectors(const Vector2& v1, const Vector2& v2);
	static float				GetSignedRadiansBetweenVectors(const Vector2& v1, const Vector2& v2);
	static Vector2		GetScreenFillScale(const cocos2d::Size& curSize);
	static const cocos2d::Size& GetScreenSize();
	static long long			GetTimeStampInMilliseconds();
	static float				GetScaleFactor();

	static float		SafeDivide(const float& up, const float& down);
	static float		ConvertMillisecondsToSeconds(long long milliseconds);
	static void			StartTimerWithCallback(cocos2d::Node* node,
							std::function<void()> callback, float time, int tag = GameConsts::ACTION_TIMER);
	static void			ClearCallbackTimers(cocos2d::Node* node, int timerTag);
	static void			ParseFloatArray(const String& data, std::vector<float>& outArray);

	static void LogVector2(const Vector2& v);
	static void LogVector3(const Vector3& v);
	static void AssertWithStrFormat(bool condition, const String& msg, const String& param);

	static MouseAxisType	ConvertStringToMouseAxisType(const String& axisName);
	static MouseButtonCode	ConvertStringToMouseButtonCode(const String& mouseButtonStr);
	static KeyCode			ConvertStringToKeyCode(const String& keyCodeStr);
	static X360Axes			ConvertStringToGameControllerAxis(const String& controllerAxisStr);
	static X360Button		ConvertStringToGameControllerButton(const String& controllerButtonStr);

	template <typename T>
	static T Avg(T a, T b);

	template <typename T, typename K>
	static bool FindKeyByValue(const std::map<T, K>& map, const K& value, T& outKey);

	template <typename T, typename K>
	static bool ContainsKey(const std::map<T, K>& map, const T& key);

	template <typename T, typename K>
	static bool ContainsKey(const cocos2d::Map<T, K>& map, const T& key);

	template <typename T>
	static void ClampValue(T& value, const T& minValue, const T& maxValue);
	
	// If value is less than threshold, then it is set to 0
	template <typename T>
	static void ThresholdAbsValue(T& value, const T& tresholdValue);

	template <typename T>
	static void WrapValue(T& value, const T& startValue, const T& endValue);
	
	template <typename T>
	static void MapValueToBidirectionalRange(const Range<T>& curRange, const Range<T>& targetRange, T& value);

private:
	Utils();
};

template <typename T>
static T Utils::Avg(T a, T b)
{
	return a + b / 2.0f;
}

template <typename T, typename K>
bool Utils::FindKeyByValue(const std::map<T, K>& map, const K& value, T& outKey)
{
	bool hasFound = false;
	for (auto& pair : map)
	{
		if (pair.second == value)
		{
			outKey = pair.first;
			hasFound = true;
		}
	}

	return hasFound;
}

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

template<typename T>
inline void Utils::ThresholdAbsValue(T& value, const T& tresholdValue)
{
	if (abs(value) < tresholdValue)
	{
		value = 0.0f;
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

template<typename T>
inline void Utils::MapValueToBidirectionalRange(const Range<T>& curRange, const Range<T>& targetRange, T& value)
{
	/* Maps value on bidirectional range which means
	 * that the value from mid point of range can grow
	 * towards range start or to range end.
	 *
	 * Example
	 * (-1920) - 1920 ---> -1 - 1 | 1920 - (-1920) = 3840
	 * 600 ---> 600/3840 = 0.15625
	 * (-1) -1 = 2 * 0.15625 = 0.3125
	*/

	// Get cur value range ratio
	T curRangeMagnitude = curRange.maxVal - curRange.minVal;
	T curValueRangeRatio = SafeDivide(value, curRangeMagnitude);

	// Use range ratio to get same portion of target range
	T targetRangeMagnitude = targetRange.maxVal - targetRange.minVal;
	value = targetRangeMagnitude * curValueRangeRatio;
}

