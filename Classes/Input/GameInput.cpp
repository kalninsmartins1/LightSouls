#include "GameInput.h"
#include "Utils/XMLLoader.h"
#include "InputTypes/Keyboard/KeyboardInput.h"
#include "InputTypes/Mouse/MouseInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

GameInput::GameInput()
{
	XMLLoader::loadInputSettings(*this, "res/Configs/Input/Input.xml");
	m_pKeyboard = KeyboardInput::create();
	m_pMouseInput = MouseInput::create();
}

GameInput* GameInput::getInstance()
{
	static GameInput instance;
	return &instance;
}

Vec2 GameInput::getInputAxis(const char* axis) const
{
	return Vec2::ZERO;
}

bool GameInput::HasActionInput(const char* action) const
{
	return true;
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

void GameInput::addActionInput(GameInputType inputType, const char* actionName, const char* inputCode)
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
		m_pKeyboard->addActionKey(actionName, keyCode);
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

void GameInput::addStateInput(GameInputType inputType, const char* actionName, const char* inputCode)
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		KeyCode keyCode = Utils::convertStringToKeyCode(inputCode);
		m_pKeyboard->addActionKey(actionName, keyCode);
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
