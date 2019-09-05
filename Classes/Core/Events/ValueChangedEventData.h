#pragma once

#include "BaseEventData.h"

class ValueChangedEventData : public BaseEventData
{
public:
	ValueChangedEventData(unsigned int senderId, float newValue, float percetageNormalized);
	
	float GetNewValue() const;
	float GetPercentageNormalized() const;

private:
	float m_newValue;
	float m_percentageNormalized;
};


