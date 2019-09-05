/*
 * \Author Martins Kalnins
 * \Date 30.03.2019
 * \Brief Class for spawning and playing visual special effects.
*/
#pragma once

#include "Classes/External/CocosEngine.h"

class AnimComponent;
class Vector2;
class String;

class VFX : public cc::Sprite 
{
public:
	VFX();

public:
	static VFX* Create(cc::Node& container, const String& pathToXML);

	void		Spawn(const Vector2& pos, float rotationAngle, const std::function<void(VFX&)>& onFinished);

private:
	bool Init(cc::Node& container, const String& pathToXML);
	void OnFinishedAnimating();

private:	
	AnimComponent*				m_animComp;
	std::function<void(VFX&)>	m_finishCallback;
};