#include "SpawnPointConfig.h"



const String& SpawnPointConfig::GetAgentType() const
{
	return m_agentType;
}

const Vector2& SpawnPointConfig::GetPosition() const
{
	return m_position;
}

int SpawnPointConfig::GetNumAgentsInRow() const
{
	return m_numAgentsInRow;
}

float SpawnPointConfig::GetAgentRowPadding() const
{
	return m_agentRowPadding;
}

float SpawnPointConfig::GetAgentColumnPadding() const
{
	return m_agentColumnPadding;
}


float SpawnPointConfig::GetSpawnDelay() const
{
	return m_spawnDelay;
}

int SpawnPointConfig::GetSpawnCount() const
{
	return m_spawnCount;
}

void SpawnPointConfig::SetAgentType(const String& agentType)
{
	m_agentType = agentType;
}

void SpawnPointConfig::SetPosition(const Vector2& position)
{
	m_position = position;
}

void SpawnPointConfig::SetNumAgentsInRow(int numAgentsInRow)
{
	m_numAgentsInRow = numAgentsInRow;
}

void SpawnPointConfig::SetAgentRowPadding(float padding)
{
	m_agentRowPadding = padding;
}

void SpawnPointConfig::SetAgentColumnPadding(float padding)
{
	m_agentColumnPadding = padding;
}

void SpawnPointConfig::SetSpawnDelay(float delay)
{
	m_spawnDelay = delay;
}

void SpawnPointConfig::SetSpawnCount(int spawnCount)
{
	m_spawnCount = spawnCount;
}


