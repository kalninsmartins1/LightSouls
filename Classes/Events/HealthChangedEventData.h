#pragma once

#include "LightSoulsTypes.h"
#include "IEventData.h"

NS_LIGHTSOULS_BEGIN

class HealthChangedEventData : public IEventData
{
public:
	HealthChangedEventData(unsigned int receiverId, float newHealthValue, float healthPercentage);

	float GetNewHealthValue() const;
	float GetHealthPercentage() const;

private:
	float m_newHealthValue;
	float m_healthPercentage;
};

NS_LIGHTSOULS_END