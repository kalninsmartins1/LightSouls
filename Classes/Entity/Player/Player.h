#pragma once

#include "cocos2d.h"


class PlayerAnimComponent;

class Player: public cocos2d::Sprite
{

public:
	static Player* create(const char* pPathToXML);
	bool init(const char* pathToXML);	
	virtual void update(float deltaTime) override;

	void setMoveSpeed(float moveSpeed);

private:	
	void onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);

	
	PlayerAnimComponent* m_pPlayerAnimComponent;
	bool m_bHasDodgeInput;
	cocos2d::Vec2 m_moveDirection;
	float m_moveSpeed;
	float m_dodgeSpeed;
};
