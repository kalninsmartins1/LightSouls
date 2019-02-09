#pragma once

#include "LightSoulsTypes.h"
#include "AEventData.h"

class ValueChangedEventData : public AEventData
{
public:
	ValueChangedEventData(unsigned int receiverId, float newValue, float percetageNormalized);
	
	float GetNewValue() const;
	float GetPercentageNormalized() const;

private:
	float m_newValue;
	float m_percentageNormalized;
};


