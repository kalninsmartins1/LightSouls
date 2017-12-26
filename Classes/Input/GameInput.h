#pragma once
#include "base/CCRef.h"


namespace cocos2d
{
	class Vec2;
}
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

class GameInput
{
public:
	static GameInput* getInstance();	

	cocos2d::Vec2 getInputAxis(const char* axis) const;
	
	// Returns true for action once when input is released
	bool HasAction(const char* action) const;	

	// Returns true while input for action is not released
	bool HasActionState(const char* action) const;
	
	void addAxisInput(GameInputType inputType, const char* actionName, const char* keyCode,
		const char* axisType, float minValue, float maxValue);
	void addActionInput(GameInputType inputType, const char* actionName, const char* buttonCode);
	void addStateInput(GameInputType inputType, const char* actionName, const char* buttonCode);

private:
	GameInput();
	void addKeyboardActionKey(const char* actionName, const char* inputCode);
	void addKeyboardStateKey(const char* actionName, const char* inputCode);

	bool m_bIsJoystickConnected;
	KeyboardInput* m_pKeyboard;
	MouseInput* m_pMouseInput;
	JoystickInput* m_pJoystickInput;
};
