#include "GameInput.h"
#include "Utils/XMLLoader.h"
#include "InputTypes/Keyboard/KeyboardInput.h"
#include "InputTypes/Mouse/MouseInput.h"
#include "Input/InputTypes/GameController/GameControllerInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

GameInput::GameInput()
{
	if (!init())
	{
		CCASSERT(false, "GameInput: Failed to initilize !");
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
	m_pGameControllerInput.reset(new (std::nothrow) GameControllerInput());

	return m_pKeyboard != nullptr && m_pMouseInput != nullptr;
}

void GameInput::update(float deltaTime)
{
	m_pMouseInput->update(deltaTime);
	m_pKeyboard->update(deltaTime);
	m_pGameControllerInput->update(deltaTime);
}

float GameInput::getInputAxis(const char* axisAction) const
{
	float value = 0;
	if (!m_pGameControllerInput->isConnected())
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
			Utils::assertWithStrFormat(false,
				"GameInput: Axis action %s for keyboard or mouse not found !",
				axisAction);
		}
	}
	else if (m_pGameControllerInput->hasAxisInput(axisAction))
	{
		value = m_pGameControllerInput->getAxisInput(axisAction);
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"GameInput: Axis action %s for controller not found !",
			axisAction);
	}

	return value;
}

bool GameInput::loadInputConfiguration(const char* pathToConfigFile)
{
	return XMLLoader::loadInputSettings(*this, pathToConfigFile);
}

bool GameInput::hasAction(const char* action) const
{
	bool bHasAction = false;
	if (!m_pGameControllerInput->isConnected())
	{
		if (m_pMouseInput->hasAction(action))
		{
			bHasAction = true;
		}
		else if (m_pKeyboard->hasAction(action))
		{
			bHasAction = true;
		}
		else
		{
			// Currently no active action with specified name has been found
		}
	}
	else if (m_pGameControllerInput->hasAction(action))
	{
		bHasAction = true;
	}

	return bHasAction;
}

bool GameInput::hasActionState(const char* action) const
{
	bool bHasActionState = false;
	if (!m_pGameControllerInput->isConnected())
	{
		if (m_pMouseInput->hasActionState(action))
		{
			bHasActionState = true;
		}
		else if (m_pKeyboard->hasAction(action))
		{
			bHasActionState = true;
		}
		else
		{
			Utils::assertWithStrFormat(false,
				"GameInput: State button %s for keyboard or mouse not found !",
				action);
		}
	}
	else if (m_pGameControllerInput->hasAction(action))
	{
		bHasActionState = true;
	}
	else
	{
		Utils::assertWithStrFormat(false,
			"GameInput: State button %s for controller not found !",
			action);
	}

	return bHasActionState;
}

void GameInput::addAxisActionInput(GameInputType inputType, const char* actionName, const char* keyCodeFromStr,
	const char* keyCodeToStr, float valueFrom, float valueTo) const
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		addKeyboardAxis(actionName, keyCodeFromStr, keyCodeToStr, valueFrom, valueTo);
		break;

	case GameInputType::Mouse:
		// Currently there is no axis input for mouse
		break;

	case GameInputType::GameController:
		addGameControllerAxis(actionName, keyCodeFromStr, valueFrom, valueTo);
		break;

	default:
		CCASSERT(false, "GameInput: [adAxisInput] unsupported input type !");
		break;
	}
}

void GameInput::addActionInput(GameInputType inputType, const char* actionName,
	const char* inputCode) const
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		addKeyboardActionKey(actionName, inputCode);
		break;

	case GameInputType::Mouse:
		addMouseActionButton(actionName, inputCode);
		break;

	case GameInputType::GameController:
		addGameControllerActionButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addStateInput(GameInputType inputType, const char* actionName,
	const char* inputCode) const
{
	switch (inputType)
	{
	case GameInputType::Keyboard:
		addKeyboardStateKey(actionName, inputCode);
		break;

	case GameInputType::Mouse:
		addMouseStateButton(actionName, inputCode);
		break;

	case GameInputType::GameController:
		addGameControllerStateButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addKeyboardActionKey(const char * actionName, const char * inputCode) const
{
	const int keyCode = static_cast<int>(Utils::convertStringToKeyCode(inputCode));
	m_pKeyboard->addActionButton(actionName, ActionButton(keyCode));
}

void GameInput::addKeyboardStateKey(const char * actionName, const char * inputCode) const
{
	const int keyCode = static_cast<int>(Utils::convertStringToKeyCode(inputCode));
	m_pKeyboard->addStateButton(actionName, StateButton(keyCode));
}

void GameInput::addKeyboardAxis(const char * actionName, const char * keyCodeFromStr,
	const char * keyCodeToStr, float valueFrom, float valueTo) const
{
	const KeyCode keyCodeFrom = Utils::convertStringToKeyCode(keyCodeFromStr);
	const KeyCode keyCodeTo = Utils::convertStringToKeyCode(keyCodeToStr);
	const KeyboardAxis key = KeyboardAxis(keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	m_pKeyboard->addKeyboardAxis(actionName, key);
}

void GameInput::addMouseActionButton(const char* actionName, const char* inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::convertStringToMouseButtonCode(inputCode));
	m_pMouseInput->addActionButton(actionName, buttonCode);
}

void GameInput::addMouseStateButton(const char* actionName, const char* inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::convertStringToMouseButtonCode(inputCode));
	m_pMouseInput->addStateButton(actionName, buttonCode);
}

void GameInput::addGameControllerActionButtons(const char* actionName, const char* inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::convertStringToGameControllerButton(inputCode));
	m_pGameControllerInput->addActionButton(actionName, buttonCode);
}

void GameInput::addGameControllerStateButtons(const char* actionName, const char* inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::convertStringToGameControllerButton(inputCode));
	m_pGameControllerInput->addStateButton(actionName, buttonCode);
}

void GameInput::addGameControllerAxis(const char* actionName, const char* axisName, float valueFrom,
	float valueTo) const
{
	const int buttonCode = static_cast<int>(
		Utils::convertStringToGameControllerAxis(axisName));

	const ControllerAxis axis = ControllerAxis(buttonCode, valueFrom, valueTo);
	m_pGameControllerInput->addAxisButton(actionName, axis);
}
