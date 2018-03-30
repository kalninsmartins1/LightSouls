#pragma once
#include "cocos2d.h"

class UIElementConfig
{
public:
	UIElementConfig();
	~UIElementConfig();

	const cocos2d::Vec2& GetAnchorPosition() const;
	const cocos2d::Vec2& GetNormalizedPosition() const;
	const std::string&	 GetPathToSprite() const;
	const cocos2d::Vec2& GetScale() const;

	void				 SetAnchorPosition(const cocos2d::Vec2& anchorPos);
	void				 SetNormalizedPosition(const cocos2d::Vec2& normalizedPos);
	void				 SetPathToSprite(const std::string& pathToSprite);
	void				 SetScale(const cocos2d::Vec2& scale);

private:
	std::string	  m_pathToSprite;
	cocos2d::Vec2 m_achorPosition;
	cocos2d::Vec2 m_normalizedPosition;
	cocos2d::Vec2 m_scale;
};