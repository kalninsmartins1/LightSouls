#include "ScoringSystem.h"

NS_LIGHTSOULS_BEGIN

ScoringSystem* ScoringSystem::GetInstance()
{
	static ScoringSystem scoreSystem;
	return &scoreSystem;
}

int ScoringSystem::GetScore() const
{
	return m_score;
}

void ScoringSystem::IncreaseScore()
{
	m_score += 10;
}

void ScoringSystem::Reset()
{
	m_score = 0;
}

ScoringSystem::ScoringSystem()
	: m_score(0)
{

}

NS_LIGHTSOULS_END