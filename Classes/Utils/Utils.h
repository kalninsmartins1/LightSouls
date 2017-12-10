#pragma once

#include "cocos2d.h"

class Utils
{	
public:
	static bool isStrEqual(const std::string& a, const std::string& b);
	static void logVec2(const cocos2d::Vec2& v);	
	static void startTimerWithCallback(cocos2d::Node* pNode,
		std::function<void()> callback, float time);

private:
	Utils();
};