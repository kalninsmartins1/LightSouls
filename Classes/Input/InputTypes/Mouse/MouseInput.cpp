#include "MouseInput.h"
#include "cocos2d.h"
#include "Utils/Utils.h"

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

void MouseInput::update(float deltaTime)
{
	// Reset action buttons after being active for one frame
	for(auto iterator = m_buttonActions.begin(); iterator != m_buttonActions.end();
		++iterator)
	{
		ActionMouseButton& button = iterator->second;
		if(button.bIsActive && button.bNeedsStateReset)
		{
			button.bIsActive = false;
			button.bNeedsStateReset = false;
			m_actionButtons[button.action] = button.bIsActive;
		}
		else if(button.bIsActive)
		{
			button.bNeedsStateReset = true;
		}
	}
}

void MouseInput::addButtonAction(const char* buttonAction, const char* buttonCode)
{
	addButtonState(buttonAction, buttonCode);
}

void MouseInput::addButtonState(const char* buttonAction, const char* inputCode)
{
	MouseButtonCode buttonCode = Utils::convertStringToMouseButtonCode(inputCode);
	StateMouseButton button = StateMouseButton(buttonAction);
	m_stateButtons.insert(std::make_pair(std::string(buttonAction), false));
	m_buttonStates.insert(std::make_pair(buttonCode, button));
}

bool MouseInput::HasAction(const std::string& action) const
{
	bool bIsActive = false;
	if(Utils::containsKey(m_actionButtons, action))
	{
		bIsActive = m_actionButtons.at(action);
	}
	return bIsActive;
}

bool MouseInput::HasActionState(const std::string& action) const 
{
	bool bIsActiveState = false;
	if(Utils::containsKey(m_stateButtons, action))
	{
		bIsActiveState = m_stateButtons.at(std::string(action));
	}
	return bIsActiveState;
}

void MouseInput::onMouseButtonDown(EventMouse* pEvent)
{
	switchButtonState(m_stateButtons, m_buttonStates, pEvent->getMouseButton(),
		true);
}

void MouseInput::onMouseButtonUp(EventMouse* pEvent)
{
	switchButtonState(m_stateButtons, m_buttonStates, pEvent->getMouseButton(),
		false);
	switchButtonState(m_actionButtons, m_buttonActions, pEvent->getMouseButton(),
		true);
}
