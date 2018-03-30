#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Player;
class ProgressBar;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

    virtual bool init() override;    
	virtual void update(float deltaTime) override;

private:
	void InitWolrdLayer();
	void InitUILayer();
	void OnPlayerHealthChanged(cocos2d::EventCustom* eventData);

private:
	Player*			m_player;
	ProgressBar*	m_healthBar;
};

#endif // __HELLOWORLD_SCENE_H__
