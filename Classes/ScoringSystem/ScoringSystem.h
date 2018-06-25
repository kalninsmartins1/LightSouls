#pragma once

#include "LightSoulsTypes.h"

NS_LIGHTSOULS_BEGIN

class ScoringSystem
{
public:
	static ScoringSystem*	GetInstance();
	int						GetScore() const;

	void					IncreaseScore();
	void					Reset();

private:
	ScoringSystem();

private:
	int		m_score;
};

NS_LIGHTSOULS_END