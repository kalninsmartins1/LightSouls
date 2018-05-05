#pragma once

#include "LightSoulsTypes.h"
#include "AEventData.h"

NS_LIGHTSOULS_BEGIN

class OnCollisionBeginEventData : public AEventData
{
public:
	OnCollisionBeginEventData(unsigned int receiverId);
};

NS_LIGHTSOULS_END