#pragma once

#include "cocos2d.h"

class InputManager
{
public:
	static InputManager* getInstance();
	void setInputContext(cocos2d::Node* pNode);		

private:	
	void onKeyboardDown(cocos2d::EventKeyboard::KeyCode keyCode) const;
	void onControllerButtonDown(cocos2d::Controller* pController, int buttonCode,
		cocos2d::Event* pEvent) const;
	void onControllerAxisEventDown(cocos2d::Controller* pController, int buttonCode,
		cocos2d::Event* pEvent) const;

private:
	//std::map<int, >
};