#pragma once
#include "2d/CCSprite.h"

class PlayerAnimComponent;
class GameInput;

class Player: public cocos2d::Sprite
{

public:
	static Player* create(const char* pPathToXML);
	Player();
	bool init(const char* pathToXML);
	virtual void update(float deltaTime) override;

	void setMoveSpeed(float moveSpeed);
	void setDodgeSpeed(float dodgeSpeed);
	void setDodgeTime(float dodgeTime);

	cocos2d::Vec2 getHeading() const;
	void setTimeBetweenComboInput(float timeBetweenComboInput);

private:
	void onDodgeFinished();
	void onAttackFinished();
	
	void manageInput();
	void lightAttack();
	void performDodge();
	void playAnimations();
	
	PlayerAnimComponent* m_pPlayerAnimComponent;
	cocos2d::Vec2 m_moveDirection;
	
	bool m_isRuning;
	bool m_bIsDodging;
	bool m_bIsAttacking;

	float m_baseMoveSpeed;
	float m_moveSpeed;
	float m_dodgeSpeed;
	float m_dodgeTime;
	float m_timeBetweenComboInput;
	float m_curTimeBetweenComboInput;
	long m_lastTimePerformedLightAttack;

	unsigned short int m_curLightAttackAnimIdx;
};
