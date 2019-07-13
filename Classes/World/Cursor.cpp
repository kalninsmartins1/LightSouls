#include "Cursor.h"
#include "Classes/Utils/XML/XMLLoader.h"
#include "Classes/Input/GameInput.h"
#include "Classes/Utils/Utils.h"

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
	Vector2 axisInput;
	GetAxisInput(axisInput);

	if (axisInput.lengthSquared() > 0.0f)
	{
		axisInput.normalize();
		UpdatePosition(axisInput);

		if (!isVisible())
		{
			setVisible(true);
		}
	}
	else if(isVisible())
	{
		setVisible(false);
	}
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
	, m_windowSize(Vector2::ZERO)
{

}

void Cursor::GetAxisInput(Vector2& outInput) const
{
	Vector2 mousePos;
	const Vector2& playerPos = m_owner.getPosition();

	if (m_gameInput.GetMousePos(mousePos))
	{
		Vector2 worldPos;
		GetWorldPos(mousePos, worldPos);
		outInput = (worldPos - playerPos).getNormalized();
	}
	else
	{
		m_gameInput.GetCombinedInputAxis(m_horizontalInputAxisName, m_verticalInputAxisName, outInput);
	}
}

void Cursor::GetWorldPos(const Vector2& mousePos, Vector2& outPos) const
{
	outPos = mousePos - (m_windowSize / 2.0f);
}

bool Cursor::Init(cc::Node& owner, const String& pathToXML)
{
	cc::Director* director = cc::Director::getInstance();
	if (director != nullptr)
	{
		m_windowSize = director->getVisibleSize();
	}

	return m_windowSize.width > 0 && XMLLoader::InitializeCursor(*this, pathToXML);
}

void Cursor::UpdatePosition(const Vector2& axisInput)
{
	const Vector2& playerPos = m_owner.getPosition();
	Vector2 cursorPos = playerPos + axisInput * m_distanceFromPlayer;
	setPosition(cursorPos);
}
