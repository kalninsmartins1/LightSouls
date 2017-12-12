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
	void setDodgeSpeed(float dodgeSpeed);
	void setDodgeTime(float dodgeTime);

	cocos2d::Vec2 getHeading();

private:	
	void onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
		cocos2d::Event* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);
	void onDodgeFinished();
	void onAttackFinished();
	
	void PerformDodge();
	
	PlayerAnimComponent* m_pPlayerAnimComponent;
	cocos2d::Vec2 m_moveDirection;
	
	bool m_bIsMoving;
	bool m_bIsDodging;
	bool m_bIsAttacking;
	
	float m_baseMoveSpeed;
	float m_moveSpeed;
	float m_dodgeSpeed;
	float m_dodgeTime;	
};
