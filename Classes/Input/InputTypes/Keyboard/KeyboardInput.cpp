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

void KeyboardInput::addKeyboardAxis(const char* actionName, const KeyboardAxis& axisKey)
{
	m_keyboardAxis.insert(std::make_pair(actionName, axisKey));

	// Both keycodes manipulate this action
	m_keyCodeToAxisAction[axisKey.keyCodeFrom] = actionName;
	m_keyCodeToAxisAction[axisKey.keyCodeTo] = actionName;
}

void KeyboardInput::addActionButton(const char* actionName, const ActionButton& actionKey)
{
	m_actionButtons.insert(std::make_pair(actionName, actionKey));
	m_buttonCodeToAction[actionKey.buttonCode] = actionName;
}

void KeyboardInput::addStateButton(const char* actionName, const StateButton& stateKey)
{
	m_stateButtons.insert(std::make_pair(actionName, stateKey));
	m_buttonCodeToStateAction[stateKey.buttonCode] = actionName;
}

void KeyboardInput::update(float deltaTime)
{
	updateActionButtonState();
	updateAxisKeyState(deltaTime);
}

void KeyboardInput::onKeyboardKeyUp(EventKeyboard::KeyCode keyCode,
	Event* pEvent)
{
	setStateKeyState(false, keyCode);
	setKeyboardAxisState(false, keyCode);

	// Action keys work once based on full press and release sequence
	setActionKeyState(true, keyCode);
}

void KeyboardInput::onKeyboardKeyDown(EventKeyboard::KeyCode keyCode,
	Event* pEvent)
{
	setStateKeyState(true, keyCode);
	setKeyboardAxisState(true, keyCode);
}

void KeyboardInput::updateAxisKeyState(float deltaTime)
{
	// Gradually reach max axis value
	for (auto& pair : m_keyboardAxis)
	{
		KeyboardAxis& axis = pair.second;

		if (axis.bFromIsPressed && !axis.bToIsPressed)
		{
			increaseAxisCurValue(axis, axis.fromValue, deltaTime);
		}		
		else if (axis.bToIsPressed && !axis.bFromIsPressed)
		{
			increaseAxisCurValue(axis, axis.toValue, deltaTime);
		}
		else if (axis.bFromIsPressed && axis.bToIsPressed)
		{
			axis.currentValue = 0;
		}
	}
}

void KeyboardInput::setActionKeyState(bool bIsActive, KeyCode keyCode)
{
	const int code = static_cast<int>(keyCode);
	if (Utils::containsKey(m_buttonCodeToAction, code))
	{
		const std::string& actionName = m_buttonCodeToAction[code];
		m_actionButtons[actionName].bIsActive = bIsActive;
	}
}

void KeyboardInput::setStateKeyState(bool bIsPressed, KeyCode keyCode)
{
	/*
	 * If the key is found then change its
	 * state to specified on.
	 */
	const int code = static_cast<int>(keyCode);
	if (Utils::containsKey(m_buttonCodeToStateAction, code))
	{
		const std::string& actionName = m_buttonCodeToStateAction[code];
		m_stateButtons[actionName].bIsPressed = bIsPressed;
	}
}

void KeyboardInput::setKeyboardAxisState(bool bIsPressed, KeyCode keyCode)
{
	if (Utils::containsKey(m_keyCodeToAxisAction, keyCode))
	{
		const std::string& action = m_keyCodeToAxisAction[keyCode];
		KeyboardAxis& axis = m_keyboardAxis[action];
		if (axis.keyCodeFrom == keyCode)
		{			
			axis.bFromIsPressed = bIsPressed;
		}
		else if (axis.keyCodeTo == keyCode)
		{
			axis.bToIsPressed = bIsPressed;
		}
		
		if (!bIsPressed)
		{
			// When button is released axis value should be set to 0
			axis.currentValue = 0;
		}
	}
}

bool KeyboardInput::hasAction(const std::string& action) const
{
	bool bHasAction = false;
	if (Utils::containsKey(m_actionButtons, action))
	{
		bHasAction = m_actionButtons.at(action).bIsActive;
	}

	return bHasAction;
}

bool KeyboardInput::hasActionState(const std::string& action) const
{
	bool bHasActionState = false;
	if (Utils::containsKey(m_stateButtons, action))
	{
		bHasActionState = m_stateButtons.at(action).bIsPressed;
	}
	return bHasActionState;
}

float KeyboardInput::getAxisInput(const std::string& axisName) const
{
	float value = 0;
	if (Utils::containsKey(m_keyboardAxis, axisName))
	{
		// Return the sum of all axis key values
		value = m_keyboardAxis.at(axisName).currentValue;
	}
	return value;
}

bool KeyboardInput::hasAxisInput(const std::string& axisName) const
{
	return Utils::containsKey(m_keyboardAxis, axisName);
}

void KeyboardInput::increaseAxisCurValue(KeyboardAxis & keyboardAxis, float value, float deltaTime)
{
	keyboardAxis.currentValue += Utils::getSign(value) * deltaTime;

	// Make sure we stay in range
	if (keyboardAxis.fromValue > keyboardAxis.toValue)
	{
		Utils::clampValue(keyboardAxis.currentValue,
			keyboardAxis.toValue, keyboardAxis.fromValue);
	}
	else
	{
		Utils::clampValue(keyboardAxis.currentValue,
			keyboardAxis.fromValue, keyboardAxis.toValue);
	}
}
