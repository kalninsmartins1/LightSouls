#include "Cursor.h"
#include "Classes/Utils/XML/XMLLoader.h"
#include "Classes/Input/GameInput.h"

Cursor* Cursor::Create(cc::Node& owner, const GameInput& gameInput, const String& pathToXML)
{
	Cursor* cursor = new (std::nothrow) Cursor(owner, gameInput);
	if (cursor != nullptr && cursor->Init(owner, pathToXML))
	{
		cursor->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(cursor);
	}

	return cursor;
}

const Vector2& Cursor::GetLookAtDirection() const
{
	return m_lookAtDirection;
}

void Cursor::Update(float deltaTime)
{
	const Vector2& playerPos = m_owner.getPosition();
	Vector2 axisInput;
	m_gameInput.GetCombinedInputAxis(m_horizontalInputAxisName, m_verticalInputAxisName, axisInput);
	axisInput.normalize();

	Vector2 cursorPos = playerPos + axisInput * m_distanceFromPlayer;
	setPosition(cursorPos);
}

void Cursor::SetDistanceFromPlayer(float distance)
{
	m_distanceFromPlayer = distance;
}

void Cursor::SetVerticalInputAxisName(const String& axisName)
{
	m_verticalInputAxisName = axisName;
}

void Cursor::SetHorizontalInputAxisName(const String& axisName)
{
	m_horizontalInputAxisName = axisName;
}

void Cursor::OnOwnerParented(cc::Node& ownerParent)
{
	ownerParent.addChild(this);
	setLocalZOrder(INT_MAX); // Cursor should be on top of everything
	setCameraMask(m_owner.getCameraMask());
}

Cursor::Cursor(const cc::Node& owner, const GameInput& gameInput)
	: m_owner(owner)
	, m_gameInput(gameInput)
	, m_distanceFromPlayer(0.0f)
{

}

bool Cursor::Init(cc::Node& owner, const String& pathToXML)
{
	return XMLLoader::InitializeCursor(*this, pathToXML);
}