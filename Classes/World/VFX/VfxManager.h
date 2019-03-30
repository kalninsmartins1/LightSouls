/*
 * \Author Martins Kalnins
 * \Date 30.03.2019
 * \Brief Class for responding to play VFX events.
*/
#pragma once

#include "LightSoulsTypes.h"

class VfxManager
{
public:
	static VfxManager Create(const String& pathToXML);

};