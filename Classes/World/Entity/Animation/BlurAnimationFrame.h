#pragma once

#include "Classes/External/CocosEngine.h"

class BlurAnimationFrame
{
public:
	BlurAnimationFrame(cocos2d::Sprite* sprite, float fadeTime, int m_targetAnimId);

public:
	int							GetTargetAnimId() const;
	bool						HasFadeOutFinished() const;
	cocos2d::Sprite*			GetSprite() const;

	void SetCurFrame(cocos2d::SpriteFrame* frame);

	void StartFadeOut();
	void StartDelayedFadeOut(float delay);
	void Update(float deltaTime);

private:
	cocos2d::Sprite*	m_sprite;
	float				m_fadeOutTime;
	float				m_delayTime;
	int					m_targetAnimId;
	bool				m_isFadeOutFinished;
	float				m_curTime;
};