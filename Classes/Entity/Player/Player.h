#pragma once

#include "cocos2d.h"

class Player: public cocos2d::Sprite
{

public:
	static Player* create(const char* pPathToXML);
	bool init(const char* pathToXML);
	void onStartMoving(cocos2d::Vec2 direction);	
	void update(float deltaTime);

private:
	void initGraphics(const char* pathToXML);
	void onCharacterMoveFinished();
	void onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);

private:	
	std::string m_baseSpriteFrameName;
	cocos2d::SpriteBatchNode* m_walkSpritebatch;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_walkFrames;		
};
