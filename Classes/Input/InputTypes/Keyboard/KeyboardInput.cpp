#include "KeyboardInput.h"


bool KeyboardInput::init()
{
	return true;
}

void KeyboardInput::addAxisKey(const char* actionName, const AxisKey& key)
{	
	m_axisKeys.insert(std::pair<std::string,
		AxisKey>(std::string(actionName), key));
}

void KeyboardInput::addActionKey(const char* actionName, const KeyCode& key)
{
	m_actionKeys.insert(std::pair<std::string,
		KeyCode>(std::string(actionName), key));
}

void KeyboardInput::addStateKey(const char* actionName, const StateKey& key)
{
	m_stateKeys.insert(std::pair<std::string,
		StateKey>(std::string(actionName), key));
}
