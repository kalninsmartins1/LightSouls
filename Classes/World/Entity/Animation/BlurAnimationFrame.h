#pragma once

#include "LightSoulsTypes.h"

class BlurAnimationFrame
{
public:
	BlurAnimationFrame(cc::Sprite* sprite, float fadeTime, int m_targetAnimId);

public:
	int					GetTargetAnimId() const;
	bool				HasFadeOutFinished() const;
	cc::Sprite*			GetSprite() const;

	void SetCurFrame(cc::SpriteFrame* frame);

	void StartFadeOut();
	void StartDelayedFadeOut(float delay);
	void Update(float deltaTime);

private:
	cc::Sprite* m_sprite;
	float		m_fadeOutTime;
	float		m_delayTime;
	int			m_targetAnimId;
	bool		m_isFadeOutFinished;
	float		m_curTime;
};