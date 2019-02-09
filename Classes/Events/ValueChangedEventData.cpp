#include "ValueChangedEventData.h"


ValueChangedEventData::ValueChangedEventData(unsigned int receiverId, float newValue, float percetageNormalized)
	: AEventData(receiverId)
	, m_newValue(newValue)	
	, m_percentageNormalized(percetageNormalized)
{

}

float ValueChangedEventData::GetNewValue() const
{
	return m_newValue;
}

float ValueChangedEventData::GetPercentageNormalized() const
{	
	return m_percentageNormalized;
}


