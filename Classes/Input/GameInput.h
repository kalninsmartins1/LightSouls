#pragma once

#include "LightSoulsTypes.h"
#include <memory>

NS_LIGHTSOULS_BEGIN

class KeyboardInput;
class MouseInput;
class GameControllerInput;

enum class GameInputType
{
	NONE,
	KEYBOARD,
	MOUSE,
	GAME_CONTROLLER,
};

class GameInput : public cocos2d::Ref
{
public:
	~GameInput();

public:
	// Returns true for action once when input is released
	bool				HasAction(const String& action) const;

	// Returns true while input for action is not released
	bool				HasActionState(const String& action) const;

	// Return current normalized axis value
	float				GetInputAxis(const String& axis) const;

	static GameInput* Create(const String& pathToConfig);
	void Update(float deltaTime);
	void AddAxisActionInput(GameInputType inputType, const String& actionName,
		const String& keyCodeFromStr, const String& keyCodeToStr,
		float valueFrom, float valueTo) const;
	void AddActionInput(GameInputType inputType, const String& actionName, const String& buttonCode) const;
	void AddStateInput(GameInputType inputType, const String& actionName, const String& buttonCode) const;
	void ResetInputState();

private:
	GameInput();

private:
	bool Init();
	bool Init(const String& pathToConfigFile);

	void AddKeyboardActionKey(const String& actionName, const String& inputCode) const;
	void AddKeyboardStateKey(const String& actionName, const String& inputCode) const;
	void AddKeyboardAxis(const String& actionName, const String& keyCodeFromStr,
		const String& keyCodeToStr, float valueFrom, float valueTo) const;

	void AddMouseActionButton(const String& actionName, const String& inputCode) const;
	void AddMouseStateButton(const String& actionName, const String& inputCode) const;

	void AddGameControllerActionButtons(const String& actionName, const String& inputCode) const;
	void AddGameControllerStateButtons(const String& actionName, const String& inputCode) const;
	void AddGameControllerAxis(const String& actionName, const String& axisName,
		float valueFrom, float valueTo) const;

private:
	std::unique_ptr<KeyboardInput>			m_keyboard;
	std::unique_ptr<MouseInput>				m_mouseInput;
	std::unique_ptr<GameControllerInput>	m_gameControllerInput;
};

NS_LIGHTSOULS_END