#pragma once

#include "LightSoulsTypes.h"
#include "AEventData.h"



class ProgressBarChangedEventData : public AEventData
{
public:
	ProgressBarChangedEventData(unsigned int receiverId, float newHealthValue, float healthPercentage);

	float GetNewValue() const;
	float GetPercentage() const;

private:
	float m_newValue;
	float m_percentage;
};


