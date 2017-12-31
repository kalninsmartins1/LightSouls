#include "GameInput.h"
#include "Utils/XMLLoader.h"
#include "InputTypes/Keyboard/KeyboardInput.h"
#include "InputTypes/Mouse/MouseInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

GameInput::GameInput():
	m_bIsJoystickConnected(false)
{
	if(!init())
	{
		cocos2d::log("GameInput: Failed to initilize !");
	}
}
GameInput* GameInput::getInstance()
{
	static GameInput instance;
	return &instance;
}

GameInput::~GameInput()
{
	m_pMouseInput.release();
	m_pKeyboard.release();
}

bool GameInput::init()
{
	// Create input generators
	m_pKeyboard.reset(new (std::nothrow) KeyboardInput());
	m_pMouseInput.reset(new (std::nothrow) MouseInput());
	//m_pJoystickInput = JoystickInput::create();	
	
	return m_pKeyboard != nullptr && m_pMouseInput != nullptr;
}

void GameInput::update(float deltaTime)
{
	m_pMouseInput->update(deltaTime);
	m_pKeyboard->update(deltaTime);
}

float GameInput::getInputAxis(const char* axisAction) const
{
	float value = 0;
	if (!m_bIsJoystickConnected)
	{
		if (m_pMouseInput->hasAxisInput(axisAction))
		{
			value = m_pMouseInput->getAxisInput(axisAction);
		}
		else if (m_pKeyboard->hasAxisInput(axisAction))
		{
			value = m_pKeyboard->getAxisInput(axisAction);
		}
		else
		{
			cocos2d::log("GameInput: Axis action %s not found !", axisAction);
		}
	}
	/*else if (m_pJoystickInput->hasAxisInput(axisAction))
	{
		value = m_pJoystickInput->getAxisInput(axisAction);
	}*/

	return value;
}

bool GameInput::loadInputConfiguration(const char* pathToConfigFile)
{
	return XMLLoader::loadInputSettings(*this, pathToConfigFile);
}

bool GameInput::hasAction(const char* action) const
{
	bool bHasAction = false;
	if (!m_bIsJoystickConnected)
	{
		if (m_pMouseInput->hasAction(action))
		{
			bHasAction = true;
		}
		else if (m_pKeyboard->hasAction(action))
		{
			bHasAction = true;
		}
	}	
	/*else if (m_pJoystickInput->hasAction(action))
	{

	}*/

	return bHasAction;
}

bool GameInput::hasActionState(const char* action) const
{
	bool bHasActionState = false;
	if (!m_bIsJoystickConnected)
	{
		if (m_pMouseInput->hasActionState(action))
		{
			bHasActionState = true;
		}
		else if (m_pKeyboard->hasAction(action))
		{
			bHasActionState = true;
		}
	}
	/*else if (m_pJoystickInput->hasAction(action))
	{

	}*/

	return bHasActionState;
}

void GameInput::addAxisActionInput(GameInputType inputType, const char* actionName, const char* keyCodeFromStr,
	const char* keyCodeToStr, float valueFrom, float valueTo)
{
	switch(inputType)
	{
	case GameInputType::Keyboard: 
		addKeyboardAxis(actionName, keyCodeFromStr, keyCodeToStr, valueFrom, valueTo);
		break;

	case GameInputType::Mouse: 
		// Currently there is no axis input for mouse
		break;

	case GameInputType::Joystick: 
		// Currently there is no axis input for joystick
		break;	

		default:
			cocos2d::log("GameInput: [adAxisInput] unsupported input type !");
			break;
	}	
}

void GameInput::addActionInput(GameInputType inputType, const char* actionName,
	const char* inputCode)
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		addKeyboardActionKey(actionName, inputCode);
		break;

	case GameInputType::Mouse:
		addMouseActionButton(actionName, inputCode);
		break;

	case GameInputType::Joystick:
		// Currently there is no joystick used
		break;

	default:
		cocos2d::log("GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addStateInput(GameInputType inputType, const char* actionName,
	const char* inputCode)
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		addKeyboardStateKey(actionName, inputCode);
		break;

	case GameInputType::Mouse:
		addMouseStateButton(actionName, inputCode);
		break;

	case GameInputType::Joystick:
		// Currently there is no joystick used
		break;

	default:
		cocos2d::log("GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addKeyboardActionKey(const char * actionName, const char * inputCode)
{
	KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
	m_pKeyboard->addActionKey(actionName, ActionKey(keyCode));
}

void GameInput::addKeyboardStateKey(const char * actionName, const char * inputCode)
{
	KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
	m_pKeyboard->addStateKey(actionName, StateKey(keyCode));
}

void GameInput::addKeyboardAxis(const char * actionName, const char * keyCodeFromStr, const char * keyCodeToStr, float valueFrom, float valueTo)
{
	const KeyCode keyCodeFrom = Utils::convertStringToKeyCode(keyCodeFromStr);
	const KeyCode keyCodeTo = Utils::convertStringToKeyCode(keyCodeToStr);
	const KeyboardAxis key = KeyboardAxis(keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	m_pKeyboard->addAxisKey(actionName, key);
}

void GameInput::addMouseActionButton(const char* actionName, const char* inputCode)
{
	MouseButtonCode buttonCode = Utils::convertStringToMouseButtonCode(inputCode);
	m_pMouseInput->addButtonAction(actionName, buttonCode);
}

void GameInput::addMouseStateButton(const char* actionName, const char* inputCode)
{
	MouseButtonCode buttonCode = Utils::convertStringToMouseButtonCode(inputCode);
	m_pMouseInput->addButtonState(actionName, buttonCode);
}
