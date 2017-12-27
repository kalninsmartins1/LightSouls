#pragma once
#include "cocos2d.h"
#include <memory>

class KeyboardInput;
class MouseInput;
class JoystickInput;

enum class GameInputType
{
	None,
	Keyboard,
	Mouse,
	Joystick
};

class GameInput: cocos2d::Ref
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
	void addAxisInput(GameInputType inputType, const char* actionName, const char* keyCode,
		const char* axisType, float minValue, float maxValue);
	void addActionInput(GameInputType inputType, const char* actionName, const char* buttonCode);
	void addStateInput(GameInputType inputType, const char* actionName, const char* buttonCode);

private:
	GameInput();
	
	bool init();
	void addKeyboardActionKey(const char* actionName, const char* inputCode);
	void addKeyboardStateKey(const char* actionName, const char* inputCode);

	bool m_bIsJoystickConnected;
	std::unique_ptr<KeyboardInput> m_pKeyboard;
	std::unique_ptr<MouseInput> m_pMouseInput;
	//std::unique_ptr<JoystickInput> m_pJoystickInput;
};
