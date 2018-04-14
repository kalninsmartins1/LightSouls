#include "GameInput.h"
#include "Utils/XML/XMLLoader.h"
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
GameInput* GameInput::GetInstance()
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

void GameInput::Update(float deltaTime)
{
	m_pMouseInput->update(deltaTime);
	m_pKeyboard->update(deltaTime);
	m_pGameControllerInput->update(deltaTime);
}

float GameInput::getInputAxis(const std::string& axisAction) const
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
			Utils::AssertWithStrFormat(false,
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
		Utils::AssertWithStrFormat(false,
			"GameInput: Axis action %s for controller not found !",
			axisAction);
	}

	return value;
}

bool GameInput::LoadInputConfiguration(const std::string& pathToConfigFile)
{
	return XMLLoader::LoadInputSettings(*this, pathToConfigFile);
}

bool GameInput::HasAction(const std::string& action) const
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

bool GameInput::hasActionState(const std::string& action) const
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
			Utils::AssertWithStrFormat(false,
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
		Utils::AssertWithStrFormat(false,
			"GameInput: State button %s for controller not found !",
			action);
	}

	return bHasActionState;
}

void GameInput::addAxisActionInput(GameInputType inputType, const std::string& actionName, const std::string& keyCodeFromStr,
	const std::string& keyCodeToStr, float valueFrom, float valueTo) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		addKeyboardAxis(actionName, keyCodeFromStr, keyCodeToStr, valueFrom, valueTo);
		break;

	case GameInputType::MOUSE:
		// Currently there is no axis input for mouse
		break;

	case GameInputType::GAME_CONTROLLER:
		addGameControllerAxis(actionName, keyCodeFromStr, valueFrom, valueTo);
		break;

	default:
		CCASSERT(false, "GameInput: [adAxisInput] unsupported input type !");
		break;
	}
}

void GameInput::addActionInput(GameInputType inputType, const std::string& actionName,
	const std::string& inputCode) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		addKeyboardActionKey(actionName, inputCode);
		break;

	case GameInputType::MOUSE:
		addMouseActionButton(actionName, inputCode);
		break;

	case GameInputType::GAME_CONTROLLER:
		addGameControllerActionButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addStateInput(GameInputType inputType, const std::string& actionName,
	const std::string& inputCode) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		addKeyboardStateKey(actionName, inputCode);
		break;

	case GameInputType::MOUSE:
		addMouseStateButton(actionName, inputCode);
		break;

	case GameInputType::GAME_CONTROLLER:
		addGameControllerStateButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::addKeyboardActionKey(const std::string& actionName, const std::string& inputCode) const
{
	const int keyCode = static_cast<int>(Utils::ConvertStringToKeyCode(inputCode));
	m_pKeyboard->addActionButton(actionName, ActionButton(keyCode));
}

void GameInput::addKeyboardStateKey(const std::string& actionName, const std::string& inputCode) const
{
	const int keyCode = static_cast<int>(Utils::ConvertStringToKeyCode(inputCode));
	m_pKeyboard->addStateButton(actionName, StateButton(keyCode));
}

void GameInput::addKeyboardAxis(const std::string& actionName, const std::string& keyCodeFromStr,
	const std::string& keyCodeToStr, float valueFrom, float valueTo) const
{
	const KeyCode keyCodeFrom = Utils::ConvertStringToKeyCode(keyCodeFromStr);
	const KeyCode keyCodeTo = Utils::ConvertStringToKeyCode(keyCodeToStr);
	const KeyboardAxis key = KeyboardAxis(keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	m_pKeyboard->addKeyboardAxis(actionName, key);
}

void GameInput::addMouseActionButton(const std::string& actionName, const std::string& inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::ConvertStringToMouseButtonCode(inputCode));
	m_pMouseInput->addActionButton(actionName, buttonCode);
}

void GameInput::addMouseStateButton(const std::string& actionName, const std::string& inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::ConvertStringToMouseButtonCode(inputCode));
	m_pMouseInput->addStateButton(actionName, buttonCode);
}

void GameInput::addGameControllerActionButtons(const std::string& actionName, const std::string& inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerButton(inputCode));
	m_pGameControllerInput->addActionButton(actionName, buttonCode);
}

void GameInput::addGameControllerStateButtons(const std::string& actionName, const std::string& inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerButton(inputCode));
	m_pGameControllerInput->addStateButton(actionName, buttonCode);
}

void GameInput::addGameControllerAxis(const std::string& actionName, const std::string& axisName, float valueFrom,
	float valueTo) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerAxis(axisName));

	const ControllerAxis axis = ControllerAxis(buttonCode, valueFrom, valueTo);
	m_pGameControllerInput->addAxisButton(actionName, axis);
}
