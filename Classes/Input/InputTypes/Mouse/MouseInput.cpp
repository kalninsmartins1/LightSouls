#include "MouseInput.h"
#include "Utils/Utils.h"
#include "Utils/XML/XMLConsts.h"

MouseInput::MouseInput()
	: m_xAxisRange(0.0f, 0.0f)
	, m_yAxisRange(0.0f, 0.0f)
	, m_lastMousePos(Vector2::ZERO)
{
	if(!Init())
	{
		CCLOG("MouseInput: Failed to initialize !");
	}
}

void MouseInput::AddAxisAction(const String& actionName, const MouseAxis& axis)
{
	m_mouseAxis[actionName] = axis;
}

void MouseInput::InitSettings(const XMLElement& element)
{
	// TODO: Consider unpacking XML values within here
}

bool MouseInput::Init()
{	
	using namespace cocos2d;
	EventListenerMouse* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(MouseInput::OnMouseButtonUp,
		this);
	mouseListener->onMouseDown = CC_CALLBACK_1(MouseInput::OnMouseButtonDown,
		this);
	mouseListener->onMouseMove = CC_CALLBACK_1(MouseInput::OnMouseMoved, this);

	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithFixedPriority(mouseListener, 1);
	InitAxisDiffValues();

	return eventDispatcher != nullptr;
}

float MouseInput::GetAxisInput(const String& axisName) const
{
	float value = 0;
	if (HasAxisInput(axisName))
	{
		value = m_mouseAxis.at(axisName).curValue;
	}
	return value;
}

const Vector2& MouseInput::GetMousePos() const
{
	return m_lastMousePos;
}

bool MouseInput::HasAxisInput(const String& axisName) const
{
	return Utils::ContainsKey(m_mouseAxis, axisName);
}

void MouseInput::InitAxisDiffValues()
{
	cc::Director* director = cc::Director::getInstance();
	if (director != nullptr)
	{
		cc::Size winSize = director->getVisibleSize();
		m_xAxisRange.maxVal = winSize.width;
		m_yAxisRange.maxVal = winSize.height;
	}
}

float MouseInput::GetPosInTargetRange(const Range<float>& curRange, const Range<float>& targetRange, const float pos)
{
	float newRangeValue = pos;
	Utils::MapValueToBidirectionalRange(curRange, targetRange, newRangeValue);
	return newRangeValue;
}

void MouseInput::UpdateMouseAxis(const Vector2& newPos)
{
	Vector2 posDiff = newPos - m_lastMousePos;
	for (auto& axis : m_mouseAxis)
	{
		MouseAxis& mouseAxis = axis.second;
		const MouseAxisType curType = mouseAxis.type;
		const Range<float> targetRange(mouseAxis.valueFrom, mouseAxis.valueTo);

		if (curType == MouseAxisType::X)
		{
			mouseAxis.curValue = GetPosInTargetRange(m_xAxisRange, targetRange, posDiff.x);
		}
		else if (curType == MouseAxisType::Y)
		{
			mouseAxis.curValue = GetPosInTargetRange(m_yAxisRange, targetRange, posDiff.y);
		}
	}

	m_lastMousePos = newPos;
}

void MouseInput::OnMouseButtonDown(cocos2d::EventMouse* pEvent)
{
	// Enable active state button
	SetStateButtonState(true, pEvent->getMouseButton());
}

void MouseInput::OnMouseButtonUp(cocos2d::EventMouse* pEvent)
{
	// Disable active state button
	SetStateButtonState(false, pEvent->getMouseButton());

	// Enable active action button
	SetActionButtonState(true, pEvent->getMouseButton());
}

void MouseInput::OnMouseMoved(cc::EventMouse* eventData)
{
	if (eventData != nullptr)
	{	
		UpdateMouseAxis(eventData->getLocationInView());
	}
}

void MouseInput::SetActionButtonState(bool isActive, const MouseButtonCode& buttonCode)	
{
	int inputCode = static_cast<int>(buttonCode);
	AInputDevice::SetActionButtonState(isActive, inputCode);
}

void MouseInput::SetStateButtonState(bool isPressed, const MouseButtonCode& buttonCode)
{
	int inputCode = static_cast<int>(buttonCode);
	AInputDevice::SetStateButtonState(isPressed, inputCode);
}