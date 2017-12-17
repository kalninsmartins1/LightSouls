#pragma once

#include <map>
#include "cocos2d.h"

typedef cocos2d::EventKeyboard::KeyCode KeyCode;
enum class AxisType
{
	X,
	Y
};

struct AxisKey
{
	KeyCode keyCode;
	AxisType axisType;
	float minValue;
	float maxValue;
};

struct StateKey
{
	KeyCode KeyCode;
	bool isCurrentlyPressed;
};

class KeyboardInput
{
public:
	bool init();
	void addAxisKey(const char* actionName, const AxisKey& key);
	void addActionKey(const char* actionName, const KeyCode& key);
	void addStateKey(const char* actionName, const StateKey& key);

private:
	std::map<std::string, AxisKey> m_axisKeys;
	std::map<std::string, StateKey> m_stateKeys;
	std::map<std::string, KeyCode> m_actionKeys;
};
