/*
 * \Author Martins Kalnins
 * \Date 30.03.2019
 * \Brief Class for spawning and playing visual special effects.
*/
#pragma once

#include "LightSoulsTypes.h"

class Vfx : public cc::Sprite 
{
public:
	static Vfx* Create(cc::Node& container, const String& pathToXML);

	void		Spawn(cc::Node& container, const Vector2& pos);

private:
	bool Init(cc::Node& container, const String& pathToXML);
	void OnFinishedAnimating();

private:	
	AnimComponent*	m_animComp;
};