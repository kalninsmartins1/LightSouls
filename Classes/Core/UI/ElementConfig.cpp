#include "ElementConfig.h"

using namespace UI;

ElementConfig::ElementConfig()
	: m_pathToImage()
	, m_achorPosition(Vector2::GetZero())
	, m_normalizedPosition(Vector2::GetZero())
	, m_scale(Vector2::GetZero())
{

}

ElementConfig::~ElementConfig()
{

}

const Vector2& ElementConfig::GetAnchorPosition() const
{
	return m_achorPosition;
}

const Vector2& ElementConfig::GetNormalizedPosition() const
{
	return m_normalizedPosition;
}

const String& ElementConfig::GetPathToImage() const
{
	return m_pathToImage;
}

const Vector2& ElementConfig::GetScale() const
{
	return m_scale;
}

void ElementConfig::SetAnchorPosition(const Vector2& anchorPos)
{
	m_achorPosition = anchorPos;
}

void ElementConfig::SetNormalizedPosition(const Vector2& normalizedPos)
{
	m_normalizedPosition = normalizedPos;
}

void ElementConfig::SetPathToImage(const String& pathToSprite)
{
	m_pathToImage = pathToSprite;
}

void ElementConfig::SetScale(const Vector2& scale)
{
	m_scale = scale;
}