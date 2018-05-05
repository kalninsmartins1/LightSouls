#pragma once

#include "LightSoulsTypes.h"
#include "AEventData.h"

NS_LIGHTSOULS_BEGIN

class OnCollisionBeginEventData : public AEventData
{
public:
	OnCollisionBeginEventData(unsigned int receiverId, const String& collidedWithName);

public:
	const String&	GetCollidedWithName() const;

private:
	String	m_collidedWithName;
};

NS_LIGHTSOULS_END