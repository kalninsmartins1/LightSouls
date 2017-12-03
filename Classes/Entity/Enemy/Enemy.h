#pragma once

#include "cocos2d.h"

class StateMachine;
class Spell;

class Enemy : public cocos2d::Sprite
{
public:
	Enemy();
	~Enemy();
	bool init(const char* pathToXML);
	void update(float deltaTime);

	float getActiveRadius() const;
	cocos2d::Vec2 getBasePosition() const;
	float getMovementSpeed() const;

private:
	StateMachine* m_pStateMachine;
	cocos2d::Vec2 m_BasePosition;
	std::vector<Spell*> m_SpellList;
	int m_Level;
	float m_Health;
	float m_MoveSpeed;
	float m_ActiveRadius;
	float m_DamageDealt;

private:
	bool initFromXML(const char* pathToXML);
};