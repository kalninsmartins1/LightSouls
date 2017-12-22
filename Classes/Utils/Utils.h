#pragma once

#include "cocos2d.h"

typedef cocos2d::EventMouse::MouseButton MouseButtonCode;
typedef cocos2d::EventKeyboard::KeyCode KeyCode;

enum class AxisType;

class Utils
{	
public:
	static bool isStrEqual(const std::string& a, const std::string& b);
	static void logVec2(const cocos2d::Vec2& v);	
	static void startTimerWithCallback(cocos2d::Node* pNode,
		std::function<void()> callback, float time);
	
	static MouseButtonCode convertStringToMouseButtonCode(const char* string);
	static KeyCode convertStringToKeyCode(const char* string);
	static AxisType convertStringToAxisType(const char* string);

	template <typename T, typename K>
	static bool containsKey(const std::map<T, K>& map, const T& key);

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
