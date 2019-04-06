#include "ProgressBar.h"
#include "Utils/XML/XMLLoader.h"
#include "UI/UIElementConfig.h"



void ProgressBar::SetCurrentValue(float value)
{
	if (value <= 1.0f && value >= 0.0f)
	{
		SetTargetValue(value);
		m_isAnimating = true;
	}
	else
	{
		CCASSERT(false, "Progress bar only accepts values in rage of 0 to 1 !");
	}	
}

void ProgressBar::MultiplyAnimationSpeed(float multiplier)
{
	m_animationSpeed *= multiplier;
}

void ProgressBar::SetAnimationSpeed(float animationSpeed)
{
	m_animationSpeed = animationSpeed;
}

ProgressBar* ProgressBar::Create(const String& pathToXML)
{
	ProgressBar* healthBar = new (std::nothrow) ProgressBar();
	if (healthBar && healthBar->Init(pathToXML))
	{
		healthBar->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(healthBar);
	}

	return healthBar;
}

ProgressBar::ProgressBar()
	: m_animationSpeed(0)
	, m_currentValue(0)
	, m_targetValue(0)
	, m_fullProgressValue(0)
	, m_isAnimating(false)
	, m_isAnimationgForward(true)
{
}

void ProgressBar::SetTargetValue(float value)
{
	m_targetValue = m_fullProgressValue * value;
	if (m_targetValue > m_currentValue)
	{
		m_isAnimationgForward = true;
	}
	else
	{
		m_isAnimationgForward = false;
	}
}

bool ProgressBar::Init(const String& pathToXML)
{
	return XMLLoader::InitializeUIProgressBar(*this, pathToXML);
}

bool ProgressBar::Init(const UIElementConfig& backgroundConfig,
	const UIElementConfig& foregroundConfig)
{	
	// Init background
	initWithFile(backgroundConfig.GetPathToSprite());
	setAnchorPoint(backgroundConfig.GetAnchorPosition());
	setNormalizedPosition(backgroundConfig.GetNormalizedPosition());

	const cocos2d::Vec2& bgScale = backgroundConfig.GetScale();
	setScale(bgScale.x, bgScale.y);

	// Init foreground
	m_progressLine = Sprite::create(foregroundConfig.GetPathToSprite());
	m_progressLine->setAnchorPoint(cocos2d::Vec2(0, 0.5f)); // Progress bar logic depends on anchor being in left most side
	m_progressLine->setNormalizedPosition(foregroundConfig.GetNormalizedPosition());

	const cocos2d::Vec2& fgScale = foregroundConfig.GetScale();
	m_progressLine->setScale(fgScale.x, fgScale.y);
	m_fullProgressValue = fgScale.x;
	m_currentValue = m_fullProgressValue; // Start out with progress bar full
	addChild(m_progressLine);

	return getTexture() != nullptr && m_progressLine->getTexture() != nullptr;
}

void ProgressBar::FinishAnimating()
{
	m_currentValue = m_targetValue;
	m_isAnimating = false;
}

void ProgressBar::Update(float deltaTime)
{
	if (m_isAnimating)
	{
		if (m_isAnimationgForward)
		{
			m_currentValue += m_animationSpeed * deltaTime;
			if (m_currentValue >= m_targetValue)
			{
				FinishAnimating();
			}
		}
		else
		{
			m_currentValue -= m_animationSpeed * deltaTime;
			if (m_currentValue <= m_targetValue)
			{
				FinishAnimating();
			}
		}
		
		m_progressLine->setScaleX(m_currentValue);
	}
}

