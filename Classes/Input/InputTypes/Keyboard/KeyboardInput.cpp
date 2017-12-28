#include "KeyboardInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

KeyboardInput::KeyboardInput()
{
	if (!init())
	{
		cocos2d::log("KeyboardInput: Error initilizing !");
	}
}

bool KeyboardInput::init()
{
	// Register for input events	
	EventListenerKeyboard* pKeyboardListener = EventListenerKeyboard::create();
	pKeyboardListener->onKeyReleased = CC_CALLBACK_2(KeyboardInput::onKeyboardKeyUp, this);
	pKeyboardListener->onKeyPressed = CC_CALLBACK_2(KeyboardInput::onKeyboardKeyDown, this);

	EventDispatcher* pEventDispatcher = Director::getInstance()->getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(pKeyboardListener, 1);

	return pEventDispatcher != nullptr;
}

void KeyboardInput::addAxisKey(const char* actionName, const AxisKey& axisKey)
{
	m_axisKeys[actionName] = axisKey;
}

void KeyboardInput::addActionKey(const char* actionName, const ActionKey& actionKey)
{
	m_actionKeys[actionName] = actionKey;
}

void KeyboardInput::addStateKey(const char* actionName, const StateKey& stateKey)
{
	m_stateKeys[actionName] = stateKey;
}

void KeyboardInput::update(float deltaTime)
{
	updateActionKeyState();
	updateAxisKeyState(deltaTime);
}

void KeyboardInput::onKeyboardKeyUp(EventKeyboard::KeyCode keyCode,
	Event* pEvent)
{
	setStateKeyState(false, keyCode);
	setAxisKeyValue(false, keyCode);

	// Action keys work once based on full press and release sequence
	setActionKeyState(true, keyCode);
}

void KeyboardInput::onKeyboardKeyDown(EventKeyboard::KeyCode keyCode,
	Event* pEvent)
{
	setStateKeyState(true, keyCode);
	setAxisKeyValue(true, keyCode);
}

void KeyboardInput::updateActionKeyState()
{
	// Make sure action keys are only active for one frame
	for (auto& pair : m_actionKeys)
	{
		ActionKey& key = pair.second;
		if (key.isActive)
		{
			key.needsStateReset = true;
		}
		else if (key.needsStateReset)
		{
			key.isActive = false;
			key.needsStateReset = false;
		}
	}
}

void KeyboardInput::updateAxisKeyState(float deltaTime)
{
	// Gradually reach max axis value
	for(auto& pair : m_axisKeys)
	{
		AxisKey& key = pair.second;
		if(key.isPressed)
		{
			if(key.currentValue < key.maxValue)
			{
				key.currentValue += deltaTime;
				Utils::clampValue(key.currentValue, key.minValue, key.maxValue);
			}
		}
		else if(key.currentValue > key.minValue)
		{
			key.currentValue = key.minValue;
		}
	}
}

void KeyboardInput::setActionKeyState(bool isActive, KeyCode keyCode)
{
	if (Utils::containsKey(m_keyCodeToAction, keyCode))
	{
		const std::string& action = m_keyCodeToAction[keyCode];
		m_actionKeys[action].isActive = isActive;
	}
}

void KeyboardInput::setStateKeyState(bool isPressed, KeyCode keyCode)
{
	/*
	 * If the key is found then change its
	 * state to specified on.
	 */
	if (Utils::containsKey(m_keyCodeToState, keyCode))
	{
		const std::string& action = m_keyCodeToState[keyCode];
		m_stateKeys[action].isCurrentlyPressed = isPressed;
	}
}

void KeyboardInput::setAxisKeyValue(bool isPressed, KeyCode keyCode)
{
	if (Utils::containsKey(m_keyCodeToAxis, keyCode))
	{
		const std::string& action = m_keyCodeToAxis[keyCode];
		m_axisKeys[action].isPressed = isPressed;
	}
}

bool KeyboardInput::hasAction(const std::string& action) const
{
	bool bHasAction = false;
	if (Utils::containsKey(m_actionKeys, action))
	{
		bHasAction = m_actionKeys.at(action).isActive;
	}

	return bHasAction;
}

bool KeyboardInput::hasActionState(const std::string& action) const
{
	bool bHasActionState = false;
	if (Utils::containsKey(m_stateKeys, action))
	{
		bHasActionState = m_stateKeys.at(action).isCurrentlyPressed;
	}
	return bHasActionState;
}

float KeyboardInput::getAxisInput(const std::string& axisName) const
{
	float value = 0;
	if (Utils::containsKey(m_axisKeys, axisName))
	{
		value = m_axisKeys.at(axisName).currentValue;
	}
	return value;
}

bool KeyboardInput::hasAxisInput(const std::string& axisName) const
{
	return Utils::containsKey(m_axisKeys, axisName);
}
