#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Player;

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

private:
	Player* m_player;	
};

#endif // __HELLOWORLD_SCENE_H__
