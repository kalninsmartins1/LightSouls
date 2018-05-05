#include "ProgressBarChangedEventData.h"

NS_LIGHTSOULS_BEGIN

ProgressBarChangedEventData::ProgressBarChangedEventData(unsigned int receiverId, float newHealthValue, float healthPercentage)
	: AEventData(receiverId)
	, m_newValue(newHealthValue)
	, m_percentage(healthPercentage)
{
}

float ProgressBarChangedEventData::GetNewValue() const
{
	return m_newValue;
}

float ProgressBarChangedEventData::GetPercentage() const
{
	return m_percentage;
}

NS_LIGHTSOULS_END
