/*!
 * \Author Martins Kalnins
 * \Date 30.03.2019
 * \Brief Class for responding to play VFX events.
*/
#pragma once

#include "Classes/External/CocosEngine.h"

class Vector2;
class String;
class VFX;

class VFXManager
{
public:
	VFXManager();

public:
	bool Init(cc::Node* container, const String& pathToXML);
	void AddVFX(const String& eventType, const String& pathToVfx);

private:
	void OnVFXEventTriggered(cc::EventCustom* eventData);
	void OnVFXFinishedCallback(VFX& vfx);
	void SpawnVFX(const String& eventType, const Vector2& pos, float rotationAngle);

private:
	cc::Node*					m_container;
	std::map<String, String>	m_eventToVFXPath;
};