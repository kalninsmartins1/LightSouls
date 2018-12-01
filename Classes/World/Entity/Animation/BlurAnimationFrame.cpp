#include "BlurAnimationFrame.h"
#include "GameConsts.h"
#include "Utils/Utils.h"

BlurAnimationFrame::BlurAnimationFrame(cc::Sprite* sprite, float fadeTime, int targetAnimId)
	: m_sprite(sprite)
	, m_fadeOutTime(fadeTime)
	, m_delayTime(0.0f)
	, m_targetAnimId(targetAnimId)
	, m_isFadeOutFinished(true)
	, m_curTime(0.0f)
{

}

int BlurAnimationFrame::GetTargetAnimId() const
{
	return m_targetAnimId;
}

bool BlurAnimationFrame::HasFadeOutFinished() const
{
	return m_isFadeOutFinished;
}

cc::Sprite* BlurAnimationFrame::GetSprite() const
{
	return m_sprite;
}

void BlurAnimationFrame::SetCurFrame(cc::SpriteFrame* frame)
{
	m_sprite->setSpriteFrame(frame);	
}

void BlurAnimationFrame::StartFadeOut()
{
	m_isFadeOutFinished = false;
	m_curTime = 0.0f;
	if (m_sprite != nullptr)
	{
		m_sprite->setOpacity(GameConsts::FULL_OPAQUE);		
	}
}

void BlurAnimationFrame::StartDelayedFadeOut(float delay)
{
	m_delayTime = delay;
}

void BlurAnimationFrame::Update(float deltaTime)
{
	if (m_delayTime > 0)
	{
		m_delayTime -= deltaTime;
		if (m_delayTime <= 0)
		{
			StartFadeOut();
		}
	}

	if (m_delayTime <= 0 && !m_isFadeOutFinished && m_sprite != nullptr)
	{
		m_curTime += deltaTime;		
		float opacity = GameConsts::FULL_OPAQUE * Utils::SafeDevide(m_curTime, m_fadeOutTime);
		m_sprite->setOpacity(opacity);
		if (m_curTime >= m_fadeOutTime)
		{
			m_isFadeOutFinished = true;
			m_curTime = 0.0f;
		}		
	}
}
