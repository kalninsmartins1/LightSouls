#pragma once
#include "cocos2d.h"
#include <memory>

class KeyboardInput;
class MouseInput;
class GameControllerInput;

enum class GameInputType
{
	None,
	Keyboard,
	Mouse,
	GameController,
};

class GameInput : cocos2d::Ref
{
public:
	static GameInput* getInstance();
	~GameInput();
	// Returns true for action once when input is released
	bool hasAction(const char* action) const;

	// Returns true while input for action is not released
	bool hasActionState(const char* action) const;

	// Return current normalized axis value
	float getInputAxis(const char* axis) const;

	bool loadInputConfiguration(const char* pathToConfigFile);
	void update(float deltaTime);
	void addAxisActionInput(GameInputType inputType, const char* actionName,
		const char* keyCodeFromStr, const char* keyCodeToStr,
		float valueFrom, float valueTo) const;
	void addActionInput(GameInputType inputType, const char* actionName, const char* buttonCode) const;
	void addStateInput(GameInputType inputType, const char* actionName, const char* buttonCode) const;

private:
	GameInput();
	bool init();

	void addKeyboardActionKey(const char* actionName, const char* inputCode) const;
	void addKeyboardStateKey(const char* actionName, const char* inputCode) const;
	void addKeyboardAxis(const char* actionName, const char* keyCodeFromStr,
		const char* keyCodeToStr, float valueFrom, float valueTo) const;

	void addMouseActionButton(const char* actionName, const char* inputCode) const;
	void addMouseStateButton(const char* actionName, const char* inputCode) const;

	void addGameControllerActionButtons(const char* actionName, const char* inputCode) const;
	void addGameControllerStateButtons(const char* actionName, const char* inputCode) const;
	void addGameControllerAxis(const char* actionName, const char* axisName,
		float valueFrom, float valueTo) const;

	std::unique_ptr<KeyboardInput> m_pKeyboard;
	std::unique_ptr<MouseInput> m_pMouseInput;
	std::unique_ptr<GameControllerInput> m_pGameControllerInput;
};
