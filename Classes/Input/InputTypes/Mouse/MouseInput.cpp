#include "MouseInput.h"
#include "cocos2d.h"
#include "Utils/Utils.h"

using namespace cocos2d;

MouseInput::MouseInput()
{
	if(!init())
	{
		cocos2d::log("MouseInput: Failed to initialize !");
	}
}

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

void MouseInput::addActionButton(const char* actionName, const ActionButton& actionButton)
{
	m_actionButtons.insert(std::make_pair(std::string(actionName), actionButton));
	m_buttonCodeToAction[actionButton.buttonCode] = actionName; 
}

void MouseInput::addStateButton(const char* actionName, const StateButton& stateButton)
{
	m_stateButtons.insert(std::make_pair(std::string(actionName), stateButton));
	m_buttonCodeToStateAction[stateButton.buttonCode] = actionName;
}

bool MouseInput::hasAction(const std::string& action) const
{
	bool bIsActive = false;
	if(Utils::containsKey(m_actionButtons, action))
	{
		bIsActive = m_actionButtons.at(action).bIsActive;
	}
	return bIsActive;
}

bool MouseInput::hasActionState(const std::string& action) const 
{
	bool bIsActiveState = false;
	if(Utils::containsKey(m_stateButtons, action))
	{
		bIsActiveState = m_stateButtons.at(action).bIsPressed;
	}
	return bIsActiveState;
}

float MouseInput::getAxisInput(const std::string& axisName) const
{
	return 0;
}

bool MouseInput::hasAxisInput(const std::string& axisName) const
{
	return false;
}

void MouseInput::onMouseButtonDown(EventMouse* pEvent)
{
	// Enable active state button
	setStateButtonState(true, pEvent->getMouseButton());
}

void MouseInput::onMouseButtonUp(EventMouse* pEvent)
{
	// Disable active state button
	setStateButtonState(false, pEvent->getMouseButton());

	// Enable active action button
	setActionButtonState(true, pEvent->getMouseButton());
}

void MouseInput::setActionButtonState(bool bIsActive, const MouseButtonCode& inputCode)	
{
	const int buttonCode = static_cast<int>(inputCode);
	if (Utils::containsKey(m_buttonCodeToAction, buttonCode))
	{
		std::string& actionName = m_buttonCodeToAction[buttonCode];
		m_actionButtons[actionName].bIsActive = bIsActive;		
	}	
}

void MouseInput::setStateButtonState(bool bIsPressed, const MouseButtonCode& inputCode)
{
	int buttonCode = static_cast<int>(inputCode);
	if (Utils::containsKey(m_buttonCodeToStateAction, buttonCode))
	{
		std::string& actionName = m_buttonCodeToStateAction[buttonCode];
		m_stateButtons[actionName].bIsPressed = bIsPressed;
	}
}
