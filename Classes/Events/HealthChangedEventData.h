#pragma once

#include "IEventData.h"

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