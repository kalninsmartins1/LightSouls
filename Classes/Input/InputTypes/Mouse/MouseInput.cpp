#include "MouseInput.h"
#include "cocos2d.h"

using namespace cocos2d;

bool MouseInput::init()
{	
	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	pMouseListener->onMouseUp = CC_CALLBACK_1(MouseInput::onMouseButtonUp,
		this);
	pMouseListener->onMouseDown = CC_CALLBACK_1(MouseInput::onMouseButtonDown,
		this);

	auto pEventDispatcher = Director::getInstance()->getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(pMouseListener, 1);

	return pEventDispatcher != nullptr;
}

void MouseInput::onMouseButtonDown(EventMouse* pEvent)
{
	switch (pEvent->getMouseButton())
	{
	case EventMouse::MouseButton::BUTTON_LEFT:
		break;
	case EventMouse::MouseButton::BUTTON_RIGHT: 
		break;
	}
}

void MouseInput::onMouseButtonUp(EventMouse* pEvent)
{
	switch (pEvent->getMouseButton())
	{
	case EventMouse::MouseButton::BUTTON_LEFT:
		break;
	case EventMouse::MouseButton::BUTTON_RIGHT:
		break;
	}
}
