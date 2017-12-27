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

void GameInput::addKeyboardActionKey(const char * actionName, const char * inputCode)
{
	KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
	m_pKeyboard->addActionKey(actionName, keyCode);
}

void GameInput::addKeyboardStateKey(const char * actionName, const char * inputCode)
{
	KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
	m_pKeyboard->addActionKey(actionName, keyCode);
}

void GameInput::update(float deltaTime)
{
	m_pMouseInput->update(deltaTime);
}

Vec2 GameInput::getInputAxis(const char* axis) const
{
	return Vec2::ZERO;
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
		//else if (m_pKeyboard->hasAction(action))
		{

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
		//else if (m_pKeyboard->hasAction(action))
		{

		}
	}
	/*else if (m_pJoystickInput->hasAction(action))
	{

	}*/

	return bHasActionState;
}

void GameInput::addAxisInput(GameInputType inputType, const char* actionName, const char* keyCodeStr,
	const char* axisTypeStr, float minValue, float maxValue)
{
	KeyCode keyCode = Utils::convertStringToKeyCode(keyCodeStr);
	AxisType axisType = Utils::convertStringToAxisType(axisTypeStr);
	AxisKey key = AxisKey(keyCode, axisType, minValue, maxValue);

	switch(inputType)
	{
	case GameInputType::Keyboard: 
		m_pKeyboard->addAxisKey(actionName, key);
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
		m_pMouseInput->addButtonAction(actionName, inputCode);
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
		m_pMouseInput->addButtonAction(actionName, inputCode);
		break;

	case GameInputType::Joystick:
		// Currently there is no joystick used
		break;

	default:
		cocos2d::log("GameInput: [addActionInput] unsupported input type !");
		break;
	}
}
