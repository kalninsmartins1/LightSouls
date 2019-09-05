#pragma once

#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/String/String.h"

class SpawnPointConfig
{
public:
	const String&	GetAgentType() const;
	const Vector2&	GetPosition() const;
	int				GetNumAgentsInRow() const;
	float			GetAgentRowPadding() const;
	float			GetAgentColumnPadding() const;
	float			GetSpawnDelay() const;
	int				GetSpawnCount() const;

	void			SetAgentType(const String& agentType);
	void			SetPosition(const Vector2& position);
	void			SetNumAgentsInRow(int agentsInRow);
	void			SetAgentRowPadding(float padding);
	void			SetAgentColumnPadding(float padding);
	void			SetSpawnDelay(float delay);
	void			SetSpawnCount(int spawnCount);

private:
	String	m_agentType;
	Vector2 m_position;
	int		m_numAgentsInRow;
	float	m_agentRowPadding;
	float	m_agentColumnPadding;
	float	m_spawnDelay;
	float	m_spawnCount;
};


