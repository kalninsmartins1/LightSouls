#include "GameInput.h"
#include "Utils/XML/XMLLoader.h"
#include "InputTypes/Keyboard/KeyboardInput.h"
#include "InputTypes/Mouse/MouseInput.h"
#include "Input/InputTypes/GameController/GameControllerInput.h"
#include "Utils/Utils.h"

NS_LIGHTSOULS_BEGIN

GameInput::GameInput()
	: m_isConfigLoaded(false)
{
	if (!Init())
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
	m_mouseInput.release();
	m_keyboard.release();
}

bool GameInput::Init()
{
	// Create input generators
	m_keyboard.reset(new (std::nothrow) KeyboardInput());
	m_mouseInput.reset(new (std::nothrow) MouseInput());
	m_gameControllerInput.reset(new (std::nothrow) GameControllerInput());

	return m_keyboard != nullptr && m_mouseInput != nullptr;
}

void GameInput::Update(float deltaTime)
{
	m_mouseInput->Update(deltaTime);
	m_keyboard->Update(deltaTime);
	m_gameControllerInput->Update(deltaTime);
}

float GameInput::GetInputAxis(const String& axisAction) const
{
	float value = 0;
	if (!m_gameControllerInput->IsConnected())
	{
		if (m_mouseInput->HasAxisInput(axisAction))
		{
			value = m_mouseInput->GetAxisInput(axisAction);
		}
		else if (m_keyboard->HasAxisInput(axisAction))
		{
			value = m_keyboard->GetAxisInput(axisAction);
		}
		else
		{
			Utils::AssertWithStrFormat(false,
				"GameInput: Axis action %s for keyboard or mouse not found !",
				axisAction);
		}
	}
	else if (m_gameControllerInput->HasAxisInput(axisAction))
	{
		value = m_gameControllerInput->GetAxisInput(axisAction);
	}
	else
	{
		Utils::AssertWithStrFormat(false,
			"GameInput: Axis action %s for controller not found !",
			axisAction);
	}

	return value;
}

bool GameInput::LoadInputConfiguration(const String& pathToConfigFile)
{
	if (!m_isConfigLoaded)
	{
		m_isConfigLoaded = XMLLoader::LoadInputSettings(*this, pathToConfigFile);
	}
	return m_isConfigLoaded;
}

bool GameInput::HasAction(const String& action) const
{
	bool hasAction = false;
	if (!m_gameControllerInput->IsConnected())
	{
		if (m_mouseInput->HasAction(action))
		{
			hasAction = true;
		}
		else if (m_keyboard->HasAction(action))
		{
			hasAction = true;
		}
		else
		{
			// Currently no active action with specified name has been found
		}
	}
	else if (m_gameControllerInput->HasAction(action))
	{
		hasAction = true;
	}

	return hasAction;
}

bool GameInput::HasActionState(const String& action) const
{
	bool hasActionState = false;
	if (!m_gameControllerInput->IsConnected())
	{
		if (m_mouseInput->HasActionState(action))
		{
			hasActionState = true;
		}
		else if (m_keyboard->HasAction(action))
		{
			hasActionState = true;
		}
		else
		{
			Utils::AssertWithStrFormat(false,
				"GameInput: State button %s for keyboard or mouse not found !",
				action);
		}
	}
	else if (m_gameControllerInput->HasAction(action))
	{
		hasActionState = true;
	}
	else
	{
		Utils::AssertWithStrFormat(false,
			"GameInput: State button %s for controller not found !",
			action);
	}

	return hasActionState;
}

void GameInput::AddAxisActionInput(GameInputType inputType, const String& actionName, const String& keyCodeFromStr,
	const String& keyCodeToStr, float valueFrom, float valueTo) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		AddKeyboardAxis(actionName, keyCodeFromStr, keyCodeToStr, valueFrom, valueTo);
		break;

	case GameInputType::MOUSE:
		// Currently there is no axis input for mouse
		break;

	case GameInputType::GAME_CONTROLLER:
		AddGameControllerAxis(actionName, keyCodeFromStr, valueFrom, valueTo);
		break;

	default:
		CCASSERT(false, "GameInput: [adAxisInput] unsupported input type !");
		break;
	}
}

void GameInput::AddActionInput(GameInputType inputType, const String& actionName,
	const String& inputCode) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		AddKeyboardActionKey(actionName, inputCode);
		break;

	case GameInputType::MOUSE:
		AddMouseActionButton(actionName, inputCode);
		break;

	case GameInputType::GAME_CONTROLLER:
		AddGameControllerActionButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::AddStateInput(GameInputType inputType, const String& actionName,
	const String& inputCode) const
{
	switch (inputType)
	{
	case GameInputType::KEYBOARD:
		AddKeyboardStateKey(actionName, inputCode);
		break;

	case GameInputType::MOUSE:
		AddMouseStateButton(actionName, inputCode);
		break;

	case GameInputType::GAME_CONTROLLER:
		AddGameControllerStateButtons(actionName, inputCode);
		break;

	default:
		CCASSERT(false, "GameInput: [addActionInput] unsupported input type !");
		break;
	}
}

void GameInput::ResetInputState()
{
	m_keyboard->ResetInputState();
	m_mouseInput->ResetInputState();
	m_gameControllerInput->ResetInputState();
}

void GameInput::AddKeyboardActionKey(const String& actionName, const String& inputCode) const
{
	const int keyCode = static_cast<int>(Utils::ConvertStringToKeyCode(inputCode));
	m_keyboard->AddActionButton(actionName, ActionButton(keyCode));
}

void GameInput::AddKeyboardStateKey(const String& actionName, const String& inputCode) const
{
	const int keyCode = static_cast<int>(Utils::ConvertStringToKeyCode(inputCode));
	m_keyboard->AddStateButton(actionName, StateButton(keyCode));
}

void GameInput::AddKeyboardAxis(const String& actionName, const String& keyCodeFromStr,
	const String& keyCodeToStr, float valueFrom, float valueTo) const
{
	const KeyCode keyCodeFrom = Utils::ConvertStringToKeyCode(keyCodeFromStr);
	const KeyCode keyCodeTo = Utils::ConvertStringToKeyCode(keyCodeToStr);
	const KeyboardAxis key = KeyboardAxis(keyCodeFrom, keyCodeTo, valueFrom, valueTo);
	m_keyboard->AddKeyboardAxis(actionName, key);
}

void GameInput::AddMouseActionButton(const String& actionName, const String& inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::ConvertStringToMouseButtonCode(inputCode));
	m_mouseInput->AddActionButton(actionName, buttonCode);
}

void GameInput::AddMouseStateButton(const String& actionName, const String& inputCode) const
{
	const int buttonCode = static_cast<int>(Utils::ConvertStringToMouseButtonCode(inputCode));
	m_mouseInput->AddStateButton(actionName, buttonCode);
}

void GameInput::AddGameControllerActionButtons(const String& actionName, const String& inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerButton(inputCode));
	m_gameControllerInput->AddActionButton(actionName, buttonCode);
}

void GameInput::AddGameControllerStateButtons(const String& actionName, const String& inputCode) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerButton(inputCode));
	m_gameControllerInput->AddStateButton(actionName, buttonCode);
}

void GameInput::AddGameControllerAxis(const String& actionName, const String& axisName, float valueFrom,
	float valueTo) const
{
	const int buttonCode = static_cast<int>(
		Utils::ConvertStringToGameControllerAxis(axisName));

	const ControllerAxis axis = ControllerAxis(buttonCode, valueFrom, valueTo);
	m_gameControllerInput->AddAxisButton(actionName, axis);
}

NS_LIGHTSOULS_END