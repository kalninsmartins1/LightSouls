#pragma once

#include "LightSoulsTypes.h"

class LSAnimation;

class LSAnimate : public cocos2d::Animate
{
public:
	static LSAnimate* Create(LSAnimation* animation);	

private:	
	virtual bool initWithAnimation(cocos2d::Animation *animation) override;
};