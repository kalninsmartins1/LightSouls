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

void MouseInput::update(float deltaTime)
{
	updateActionButtonState();
}

void MouseInput::addButtonAction(const char* buttonAction, const MouseButtonCode& buttonCode)
{	
	m_actionButtons[buttonAction] = false;
	m_buttonActions[buttonCode] = ActionMouseButton(buttonAction);
}

void MouseInput::addButtonState(const char* buttonAction, const MouseButtonCode& buttonCode)
{	
	StateMouseButton button = StateMouseButton(buttonAction);
	m_stateButtons[buttonAction] = false;
	m_buttonStates[buttonCode] = button;
}

bool MouseInput::hasAction(const std::string& action) const
{
	bool bIsActive = false;
	if(Utils::containsKey(m_actionButtons, action))
	{
		bIsActive = m_actionButtons.at(action);
	}
	return bIsActive;
}

bool MouseInput::hasActionState(const std::string& action) const 
{
	bool bIsActiveState = false;
	if(Utils::containsKey(m_stateButtons, action))
	{
		bIsActiveState = m_stateButtons.at(std::string(action));
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
	switchStateButtonState(m_stateButtons, m_buttonStates, pEvent->getMouseButton(),
		true);	
}

void MouseInput::onMouseButtonUp(EventMouse* pEvent)
{
	// Disable active state button
	switchStateButtonState(m_stateButtons, m_buttonStates, pEvent->getMouseButton(),
		false);	

	// Enable active action button
	switchActionButtonState(m_actionButtons, m_buttonActions, pEvent->getMouseButton(),
		true);
}	

void MouseInput::updateActionButtonState()
{	
	if(!m_buttonActions.empty())
	{
		for (auto& pair : m_buttonActions)
		{
			ActionMouseButton& button = pair.second;
			if (button.bIsActive && button.bNeedsStateReset)
			{
				// Reset action buttons after being active for one frame
				button.bIsActive = false;
				button.bNeedsStateReset = false;
				m_actionButtons[button.action] = button.bIsActive;
			}
			else if (button.bIsActive)
			{
				button.bNeedsStateReset = true;
			}
		}
	}
}

void MouseInput::switchActionButtonState(ActionStateMap& stateMap,
	ActionButtonCodeMap& codeMap, MouseButtonCode buttonCode, bool bIsActive)
{
	if (Utils::containsKey(codeMap, buttonCode))
	{
		ActionMouseButton& button = codeMap[buttonCode];
		button.bIsActive = bIsActive;
		stateMap[button.action] = button.bIsActive;
	}
}

void MouseInput::switchStateButtonState(ActionStateMap & stateMap,
	StateButtonCodeMap& codeMap, MouseButtonCode buttonCode, bool bIsPressed)
{
	if (Utils::containsKey(codeMap, buttonCode))
	{
		StateMouseButton& button = codeMap[buttonCode];
		button.bIsPressed = bIsPressed;
		stateMap[button.actionName] = button.bIsPressed;
	}
}
