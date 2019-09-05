#include "GameControllerInput.h"
#include "Utils/Utils.h"
#include "Classes/Core/String/String.h"

bool GameControllerInput::s_isConnected = false;

GameControllerInput::GameControllerInput()
{
	if (!Init())
	{
		CCASSERT(false, "GameControllerInput: Failed to initialize !");
	}
	else
	{
		cocos2d::Controller::startDiscoveryController();
	}
}

GameControllerInput::~GameControllerInput()
{
	// Stop controller updating and release related resources
	cocos2d::Controller::stopDiscoveryController();
}

bool GameControllerInput::Init()
{
	// Register for input events
	using namespace cocos2d;
	EventListenerController* controllerListener = EventListenerController::create();
	controllerListener->onConnected = CC_CALLBACK_0(GameControllerInput::OnControllerConnected, this);
	controllerListener->onDisconnected = CC_CALLBACK_0(GameControllerInput::OnControllerDisconnected, this);
	controllerListener->onKeyDown = CC_CALLBACK_3(GameControllerInput::OnButtonDown, this);
	controllerListener->onKeyUp = CC_CALLBACK_3(GameControllerInput::OnButtonUp, this);
	controllerListener->onAxisEvent = CC_CALLBACK_3(GameControllerInput::OnAxisInput, this);

	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithFixedPriority(controllerListener, 1);

	return eventDispatcher != nullptr;
}

float GameControllerInput::GetAxisInput(const String& axisName) const
{
	float value = 0;
	if (HasAxisInput(axisName))
	{
		value = m_controllerAxis.at(axisName).curValue;
	}

	return value;
}

bool GameControllerInput::HasAxisInput(const String& axisName) const
{
	bool hasAxisInput = false;
	if (Utils::ContainsKey(m_controllerAxis, axisName))
	{
		hasAxisInput = true;
	}

	return hasAxisInput;
}

void GameControllerInput::AddAxisButton(const String& actionName,
	const ControllerAxis& axisButton)
{
	m_buttonCodeToAxisAction[axisButton.buttonCode] = actionName;
	m_controllerAxis.insert(std::make_pair(actionName, axisButton));
}

void GameControllerInput::ResetInputState()
{
	AInputDevice::ResetInputState();
	for (auto& axis : m_controllerAxis)
	{
		axis.second.isPressed = false;
		axis.second.curValue = 0;
	}
}

bool GameControllerInput::IsConnected() const
{
	return s_isConnected;
}

void GameControllerInput::OnControllerConnected()
{
	s_isConnected = true;
}

void GameControllerInput::OnControllerDisconnected()
{
	s_isConnected = false;
}

void GameControllerInput::OnButtonDown(cocos2d::Controller* pController, int buttonCode,
	cocos2d::Event* pEvent)
{
	SetStateButtonState(true, buttonCode);
}

void GameControllerInput::OnButtonUp(cocos2d::Controller* pController, int buttonCode,
	cocos2d::Event* pEvent)
{
	SetStateButtonState(false, buttonCode);
	SetActionButtonState(true, buttonCode);
}

void GameControllerInput::OnAxisInput(cocos2d::Controller* pController, int buttonCode,
	cocos2d::Event* pEvent)
{
	auto buttonStatus = pController->getKeyStatus(buttonCode);
	SetAxisInputState(buttonStatus.value, buttonCode);
}

void GameControllerInput::SetAxisInputState(float value, int buttonCode)
{
	if (Utils::ContainsKey(m_buttonCodeToAxisAction, buttonCode))
	{
		String& actionName = m_buttonCodeToAxisAction[buttonCode];
		ControllerAxis& axis = m_controllerAxis[actionName];
		if (abs(value) > 0)
		{
			axis.curValue = value;
			axis.isPressed = true;

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
			axis.isPressed = false;
		}
	}
}

