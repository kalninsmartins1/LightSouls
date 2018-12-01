#include "BlurAnimation.h"
#include "GameConsts.h"
#include "BlurAnimationFrame.h"
#include "World/Entity/Components/AnimComponent.h"
#include "World/Entity/Entity.h"
#include "Utils/Utils.h"

BlurAnimation::BlurAnimation(const AnimComponent& animComp)
	: m_animComp(animComp)	
{
	
}

void BlurAnimation::AddFrame(float fadeTime, int targetAnimationId, float delay)
{	
	cc::Sprite* sprite = cc::Sprite::createWithSpriteFrame(m_animComp.GetCurrentSpriteFrame());
	CC_ASSERT(sprite != nullptr);
	const Entity& ownerEntity = m_animComp.GetOwnerEntity();
	sprite->setCameraMask(ownerEntity.getCameraMask());
	sprite->setScale(ownerEntity.getScaleX(), ownerEntity.getScaleY());
	m_blurFrames.push_back(BlurAnimationFrame(sprite, fadeTime, targetAnimationId));
	BlurAnimationFrame& blurFrame = m_blurFrames.back();
	
	LoadNextFrame(blurFrame);	
	if (delay > 0)
	{
		blurFrame.StartDelayedFadeOut(delay);
	}
	else
	{
		blurFrame.StartFadeOut();
	}

	cc::Node* spriteContainer = m_animComp.GetSpriteContainer();
	CC_ASSERT(spriteContainer != nullptr);
	spriteContainer->addChild(blurFrame.GetSprite());
}

void BlurAnimation::Update(float deltaTime)
{
	for (unsigned int index = 0; index < m_blurFrames.size(); ++index)
	{
		BlurAnimationFrame& curFrame = m_blurFrames[index];
		if (curFrame.HasFadeOutFinished())
		{
			int targetAnimId = curFrame.GetTargetAnimId();
			if (m_animComp.IsCurrrentlyPlayingAnim(targetAnimId))
			{
				// Load next frame
				LoadNextFrame(curFrame);
				curFrame.StartFadeOut();
			}
			else
			{
				// Destroy
				RemoveBlurFrame(index);
				--index;
			}
		}
		else
		{
			curFrame.Update(deltaTime);
		}
	}
}

void BlurAnimation::LoadNextFrame(BlurAnimationFrame& blurFrame)
{	
	blurFrame.SetCurFrame(m_animComp.GetCurrentSpriteFrame());

	const Entity& owner = m_animComp.GetOwnerEntity();
	blurFrame.GetSprite()->setPosition(Vector2(owner.getPosition()));
}

void BlurAnimation::RemoveBlurFrame(int index)
{
	cc::Node* spriteContainer = m_animComp.GetSpriteContainer();
	CC_ASSERT(spriteContainer != nullptr);

	BlurAnimationFrame& curFrame = m_blurFrames[index];
	spriteContainer->removeChild(curFrame.GetSprite());
	m_blurFrames.erase(m_blurFrames.begin() + index);
}