#include "UIElementConfig.h"

NS_LIGHTSOULS_BEGIN

UIElementConfig::UIElementConfig()
	: m_pathToSprite()
	, m_achorPosition(Vector2::ZERO)
	, m_normalizedPosition(Vector2::ZERO)
	, m_scale(Vector2::ZERO)
{

}

UIElementConfig::~UIElementConfig()
{

}

const Vector2& UIElementConfig::GetAnchorPosition() const
{
	return m_achorPosition;
}

const Vector2& UIElementConfig::GetNormalizedPosition() const
{
	return m_normalizedPosition;
}

const String& UIElementConfig::GetPathToSprite() const
{
	return m_pathToSprite;
}

const Vector2& UIElementConfig::GetScale() const
{
	return m_scale;
}

void UIElementConfig::SetAnchorPosition(const Vector2& anchorPos)
{
	m_achorPosition = anchorPos;
}

void UIElementConfig::SetNormalizedPosition(const Vector2& normalizedPos)
{
	m_normalizedPosition = normalizedPos;
}

void UIElementConfig::SetPathToSprite(const String& pathToSprite)
{
	m_pathToSprite = pathToSprite;
}

void UIElementConfig::SetScale(const Vector2& scale)
{
	m_scale = scale;
}

NS_LIGHTSOULS_END