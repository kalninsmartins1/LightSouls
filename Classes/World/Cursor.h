/*!
 * \Author	Martins Kalnins
 * \Date	22.06.2019
 * \Brief	Class to handle player heading cursor.
*/
#pragma once

#include "LightSoulsTypes.h"

class GameInput;

class Cursor : public cc::Sprite
{
public:
	static Cursor* Create(cc::Node& owner, const GameInput& gameInput, const String& pathToXML);

	const Vector2& GetLookAtDirection() const;

	void SetDistanceFromPlayer(float distance);
	void SetVerticalInputAxisName(const String& axisName);
	void SetHorizontalInputAxisName(const String& axisName);

	void Update(float deltaTime);
	void OnOwnerParented(cc::Node& ownerParent);

private:
	Cursor(const cc::Node& owner, const GameInput& gameInput);

private:
	void GetAxisInput(Vector2& outInput) const;
	// This calculation is based on character being in center
	void GetWorldPos(const Vector2& mousePos, Vector2& outPos) const;

	bool Init(cc::Node& owner, const String& pathToXML);

	void UpdatePosition(const Vector2& axisInput);

private:
	const cc::Node&		m_owner;
	const GameInput&	m_gameInput;

	float           m_distanceFromPlayer;
	Vector2			m_lookAtDirection;
	String			m_verticalInputAxisName;
	String			m_horizontalInputAxisName;
	cc::Size		m_windowSize;
};