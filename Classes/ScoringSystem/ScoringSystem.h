#pragma once

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


