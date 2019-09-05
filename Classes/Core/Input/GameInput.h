#pragma once

#include <memory>
#include "Classes/External/CocosEngine.h"

namespace tinyxml2 
{
	class XMLElement;
};

class KeyboardInput;
class MouseInput;
class GameControllerInput;
class Vector2;
class String;

using XMLElement = tinyxml2::XMLElement;

enum class GameInputType
{
	NONE,
	KEYBOARD,
	MOUSE,
	GAME_CONTROLLER,
};

class GameInput : public cc::Ref
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

	// Returs axisA as x and axisB as y current normalized values
	void				GetCombinedInputAxis(const String& axisA, const String& axisB, Vector2& outResult) const;

	// Returns true if mouse is currently being used
	bool				GetMousePos(Vector2& outPos) const;

	static GameInput* Create(const String& pathToConfig);
	void Update(float deltaTime);
	void AddAxisActionInput(GameInputType inputType, const String& actionName,
		const String& keyCodeFromStr, const String& keyCodeToStr,
		float valueFrom, float valueTo) const;
	void AddActionInput(GameInputType inputType, const String& actionName, const String& buttonCode) const;
	void AddStateInput(GameInputType inputType, const String& actionName, const String& buttonCode) const;
	void ResetInputState();
	void InitMouseSettings(const XMLElement& element);

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
	void AddMouseAxis(const String& actionName, const String& axisName,
		float valueFrom, float valueTo) const;

	void AddGameControllerActionButtons(const String& actionName, const String& inputCode) const;
	void AddGameControllerStateButtons(const String& actionName, const String& inputCode) const;
	void AddGameControllerAxis(const String& actionName, const String& axisName,
		float valueFrom, float valueTo) const;

private:
	std::unique_ptr<KeyboardInput>			m_keyboard;
	std::unique_ptr<MouseInput>				m_mouseInput;
	std::unique_ptr<GameControllerInput>	m_gameControllerInput;
};

