#pragma once
#include "cocos2d.h"
#include <memory>

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

class GameInput : cocos2d::Ref
{
public:
	static GameInput* getInstance();
	~GameInput();
	// Returns true for action once when input is released
	bool hasAction(const std::string& action) const;

	// Returns true while input for action is not released
	bool hasActionState(const std::string& action) const;

	// Return current normalized axis value
	float getInputAxis(const std::string& axis) const;

	bool loadInputConfiguration(const std::string& pathToConfigFile);
	void update(float deltaTime);
	void addAxisActionInput(GameInputType inputType, const std::string& actionName,
		const std::string& keyCodeFromStr, const std::string& keyCodeToStr,
		float valueFrom, float valueTo) const;
	void addActionInput(GameInputType inputType, const std::string& actionName, const std::string& buttonCode) const;
	void addStateInput(GameInputType inputType, const std::string& actionName, const std::string& buttonCode) const;

private:
	GameInput();
	bool init();

	void addKeyboardActionKey(const std::string& actionName, const std::string& inputCode) const;
	void addKeyboardStateKey(const std::string& actionName, const std::string& inputCode) const;
	void addKeyboardAxis(const std::string& actionName, const std::string& keyCodeFromStr,
		const std::string& keyCodeToStr, float valueFrom, float valueTo) const;

	void addMouseActionButton(const std::string& actionName, const std::string& inputCode) const;
	void addMouseStateButton(const std::string& actionName, const std::string& inputCode) const;

	void addGameControllerActionButtons(const std::string& actionName, const std::string& inputCode) const;
	void addGameControllerStateButtons(const std::string& actionName, const std::string& inputCode) const;
	void addGameControllerAxis(const std::string& actionName, const std::string& axisName,
		float valueFrom, float valueTo) const;

	std::unique_ptr<KeyboardInput> m_pKeyboard;
	std::unique_ptr<MouseInput> m_pMouseInput;
	std::unique_ptr<GameControllerInput> m_pGameControllerInput;
};
