#pragma once
#include "base/CCRef.h"


namespace cocos2d
{
	class Vec2;
}
class KeyboardInput;
class MouseInput;
class JoystickInput;

class GameInput
{
public:
	static GameInput* getInstance();
	
	bool init(const char* configFilePath);

	cocos2d::Vec2 getInputAxis(const char* axis) const;
	bool HasActionInput(const char* action) const;	
	
	void addAxisInput(const char* actionName, const char* keyCode,
		const char* axisType, float minValue, float maxValue);
	void addActionInput(const char* actionName, const char* buttonCode);
	void addStateInput(const char* actionName, const char* buttonCode);


private:
	GameInput();
	KeyboardInput* m_pKeyboard;
	MouseInput* m_pMouseInput;
	JoystickInput* m_pJoystickInput;
};
