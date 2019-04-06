/*
 * \Author Martins Kalnins
 * \Date 30.03.2019
 * \Brief Class for responding to play VFX events.
*/
#pragma once

#include "LightSoulsTypes.h"

class VFX;

class VFXManager
{
public:
	VFXManager();

public:
	bool Init(cc::Node* container, const String& pathToXML);
	void AddVFX(const String& eventType, const String& pathToVfx);
	void Cleanup();

private:
	void OnVFXEventTriggered(cc::EventCustom* eventData);

private:
	cc::Node*				m_container;
	std::map<String, VFX*>	m_eventToVFX;
};