#include "MouseInput.h"
#include "Utils/Utils.h"

MouseInput::MouseInput()
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

bool MouseInput::HasAxisInput(const String& axisName) const
{
	return Utils::ContainsKey(m_mouseAxis, axisName);;
}

void MouseInput::UpdateMouseAxis(const Vector2& moveDirection)
{
	for (auto& axis : m_mouseAxis)
	{
		MouseAxis& mouseAxis = axis.second;
		const MouseAxisType curType = mouseAxis.type;
		const float minValue = mouseAxis.valueFrom;
		const float maxValue = mouseAxis.valueTo;
		float newValue = 0.0f;

		if (curType == MouseAxisType::X)
		{
			newValue = moveDirection.x;
		}
		else if (curType == MouseAxisType::Y)
		{
			newValue = moveDirection.y;
		}

		Utils::ClampValue(newValue, minValue, maxValue);
		mouseAxis.curValue = newValue;
	}
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
		UpdateMouseAxis(eventData->getDelta());
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