#include "GameControllerInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

GameControllerInput::GameControllerInput()
{
	m_bIsConnected = false;

	if (!init())
	{
		CCASSERT(false, "GameControllerInput: Failed to initialize !");
	}
	else
	{
		Controller::startDiscoveryController();
	}
}

GameControllerInput::~GameControllerInput()
{
	// Stop controller updating and release related resources
	Controller::stopDiscoveryController();
}

bool GameControllerInput::init()
{
	// Register for input events
	EventListenerController* controllerListener = EventListenerController::create();
	controllerListener->onConnected = CC_CALLBACK_0(GameControllerInput::onControllerConnected, this);
	controllerListener->onDisconnected = CC_CALLBACK_0(GameControllerInput::onControllerDisconnected, this);
	controllerListener->onKeyDown = CC_CALLBACK_3(GameControllerInput::onButtonDown, this);
	controllerListener->onKeyUp = CC_CALLBACK_3(GameControllerInput::onButtonUp, this);
	controllerListener->onAxisEvent = CC_CALLBACK_3(GameControllerInput::onAxisInput, this);

	EventDispatcher* pEventDispatcher = Director::getInstance()->getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(controllerListener, 1);

	return pEventDispatcher != nullptr;
}

bool GameControllerInput::hasAction(const std::string& action) const
{
	bool bHasAction = false;
	if (Utils::ContainsKey(m_actionButtons, action))
	{
		bHasAction = m_actionButtons.at(action).bIsActive;
	}

	return bHasAction;
}

bool GameControllerInput::hasActionState(const std::string& action) const
{
	bool bHasActionState = false;
	if (Utils::ContainsKey(m_stateButtons, action))
	{
		bHasActionState = m_stateButtons.at(action).bIsPressed;
	}

	return bHasActionState;
}

float GameControllerInput::getAxisInput(const std::string& axisName) const
{
	float value = 0;
	if (Utils::ContainsKey(m_controllerAxis, axisName))
	{
		value = m_controllerAxis.at(axisName).curValue;
	}

	return value;
}

bool GameControllerInput::hasAxisInput(const std::string& axisName) const
{
	bool bHasAxisInput = false;
	if (Utils::ContainsKey(m_controllerAxis, axisName))
	{
		bHasAxisInput = m_controllerAxis.at(axisName).bIsPressed;
	}

	return bHasAxisInput;
}

void GameControllerInput::addAxisButton(const std::string& actionName,
	const ControllerAxis& axisButton)
{
	m_buttonCodeToAxisAction[axisButton.buttonCode] = actionName;
	m_controllerAxis.insert(std::make_pair(actionName, axisButton));
}

void GameControllerInput::addActionButton(const std::string& actionName, const ActionButton& actionButton)
{
	m_actionButtons.insert(std::make_pair(actionName, actionButton));
	m_buttonCodeToAction[actionButton.buttonCode] = actionName;
}

void GameControllerInput::addStateButton(const std::string& actionName, const StateButton& stateButton)
{
	m_stateButtons.insert(std::make_pair(actionName, stateButton));
	m_buttonCodeToStateAction[stateButton.buttonCode] = actionName;
}

bool GameControllerInput::isConnected() const
{
	return m_bIsConnected;
}

void GameControllerInput::onControllerConnected()
{
	m_bIsConnected = true;
}

void GameControllerInput::onControllerDisconnected()
{
	m_bIsConnected = false;
}

void GameControllerInput::onButtonDown(Controller* pController, int buttonCode,
	Event* pEvent)
{
	setStateButtonState(true, buttonCode);
}

void GameControllerInput::onButtonUp(Controller* pController, int buttonCode,
	Event* pEvent)
{
	setStateButtonState(false, buttonCode);
	setActionButtonState(true, buttonCode);
}

void GameControllerInput::onAxisInput(Controller* pController, int buttonCode,
	Event* pEvent)
{
	auto buttonStatus = pController->getKeyStatus(buttonCode);
	setAxisInputState(buttonStatus.value, buttonCode);
}

void GameControllerInput::setActionButtonState(bool bIsActive, int buttonCode)
{
	if (Utils::ContainsKey(m_buttonCodeToAction, buttonCode))
	{
		std::string& actionName = m_buttonCodeToAction[buttonCode];
		m_actionButtons[actionName].bIsActive = bIsActive;
	}
}

void GameControllerInput::setStateButtonState(bool bIsPressed, int buttonCode)
{
	if (Utils::ContainsKey(m_buttonCodeToStateAction, buttonCode))
	{
		std::string& actionName = m_buttonCodeToStateAction[buttonCode];
		m_stateButtons[actionName].bIsPressed = bIsPressed;
	}
}

void GameControllerInput::setAxisInputState(float value, int buttonCode)
{
	if (Utils::ContainsKey(m_buttonCodeToAxisAction, buttonCode))
	{
		std::string& actionName = m_buttonCodeToAxisAction[buttonCode];
		ControllerAxis& axis = m_controllerAxis[actionName];
		if (abs(value) > 0)
		{
			axis.curValue = value;
			axis.bIsPressed = true;

			// Clamp axis value to specified range
			if (axis.toValue > axis.fromValue)
			{
				Utils::ClampValue(axis.curValue, axis.fromValue, axis.toValue);
			}
			else
			{
				Utils::ClampValue(axis.curValue, axis.toValue, axis.fromValue);
			}
		}
		else
		{
			axis.curValue = 0;
			axis.bIsPressed = false;
		}
	}
}
