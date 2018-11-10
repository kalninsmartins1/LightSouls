#pragma once

#include "LightSoulsTypes.h"



class UIElementConfig
{
public:
	UIElementConfig();
	~UIElementConfig();

	const Vector2& GetAnchorPosition() const;
	const Vector2& GetNormalizedPosition() const;
	const String&  GetPathToSprite() const;
	const Vector2& GetScale() const;

	void				 SetAnchorPosition(const Vector2& anchorPos);
	void				 SetNormalizedPosition(const Vector2& normalizedPos);
	void				 SetPathToSprite(const String& pathToSprite);
	void				 SetScale(const Vector2& scale);

private:
	String	m_pathToSprite;
	Vector2 m_achorPosition;
	Vector2 m_normalizedPosition;
	Vector2 m_scale;
};


