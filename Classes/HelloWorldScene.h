#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Player;
class CameraController;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

    virtual bool init();    
	virtual void update(float deltaTime) override;

private:
	Player* m_pPlayer;
	
	void initWolrdLayer();
	void initUILayer();
};

#endif // __HELLOWORLD_SCENE_H__
