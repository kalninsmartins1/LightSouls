#include "MouseInput.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

MouseInput::MouseInput()
{
	if(!Init())
	{
		CCLOG("MouseInput: Failed to initialize !");
	}
}

bool MouseInput::Init()
{	
	using namespace cocos2d;
	EventListenerMouse* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(MouseInput::OnMouseButtonUp,
		this);
	mouseListener->onMouseDown = CC_CALLBACK_1(MouseInput::OnMouseButtonDown,
		this);

	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithFixedPriority(mouseListener, 1);
		
	return eventDispatcher != nullptr;
}

float MouseInput::GetAxisInput(const String& axisName) const
{
	return 0;
}

bool MouseInput::HasAxisInput(const String& axisName) const
{
	return false;
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

NS_LIGHTSOULS_END