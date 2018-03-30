#include "HealthChangedEventData.h"

HealthChangedEventData::HealthChangedEventData(unsigned int receiverId, float newHealthValue, float healthPercentage)
	: IEventData(receiverId)
	, m_newHealthValue(newHealthValue)
	, m_healthPercentage(healthPercentage)
{
}

float HealthChangedEventData::GetNewHealthValue() const
{
	return m_newHealthValue;
}

float HealthChangedEventData::GetHealthPercentage() const
{
	return m_healthPercentage;
}
