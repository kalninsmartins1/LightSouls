#include "UIElementConfig.h"

UIElementConfig::UIElementConfig()
	: m_pathToSprite()
	, m_achorPosition(cocos2d::Vec2::ZERO)
	, m_normalizedPosition(cocos2d::Vec2::ZERO)
	, m_scale(cocos2d::Vec2::ZERO)
{

}

UIElementConfig::~UIElementConfig()
{

}

const cocos2d::Vec2& UIElementConfig::GetAnchorPosition() const
{
	return m_achorPosition;
}

const cocos2d::Vec2& UIElementConfig::GetNormalizedPosition() const
{
	return m_normalizedPosition;
}

const std::string& UIElementConfig::GetPathToSprite() const
{
	return m_pathToSprite;
}

const cocos2d::Vec2& UIElementConfig::GetScale() const
{
	return m_scale;
}

void UIElementConfig::SetAnchorPosition(const cocos2d::Vec2& anchorPos)
{
	m_achorPosition = anchorPos;
}

void UIElementConfig::SetNormalizedPosition(const cocos2d::Vec2& normalizedPos)
{
	m_normalizedPosition = normalizedPos;
}

void UIElementConfig::SetPathToSprite(const std::string& pathToSprite)
{
	m_pathToSprite = pathToSprite;
}

void UIElementConfig::SetScale(const cocos2d::Vec2& scale)
{
	m_scale = scale;
}