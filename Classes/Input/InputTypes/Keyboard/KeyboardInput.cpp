#include "KeyboardInput.h"
#include "Utils/Utils.h"

KeyboardInput::KeyboardInput()
{
	if (!Init())
	{
		CCLOG("KeyboardInput: Error initilizing !");
	}
}

bool KeyboardInput::Init()
{
	// Register for input events	
	using namespace cocos2d;
	EventListenerKeyboard* pKeyboardListener = EventListenerKeyboard::create();
	pKeyboardListener->onKeyReleased = CC_CALLBACK_2(KeyboardInput::OnKeyboardKeyUp, this);
	pKeyboardListener->onKeyPressed = CC_CALLBACK_2(KeyboardInput::OnKeyboardKeyDown, this);

	EventDispatcher* pEventDispatcher = Director::getInstance()->getEventDispatcher();
	pEventDispatcher->addEventListenerWithFixedPriority(pKeyboardListener, 1);

	return pEventDispatcher != nullptr;
}

void KeyboardInput::AddKeyboardAxis(const String& actionName, const KeyboardAxis& axisKey)
{
	m_keyboardAxis.insert(std::make_pair(actionName, axisKey));

	// Both keycodes manipulate this action
	m_keyCodeToAxisAction[axisKey.keyCodeFrom] = actionName;
	m_keyCodeToAxisAction[axisKey.keyCodeTo] = actionName;
}

void KeyboardInput::Update(float deltaTime)
{
	UpdateActionButtonState();
	UpdateAxisKeyState(deltaTime);
}

void KeyboardInput::ResetInputState()
{
	AInputDevice::ResetInputState();
	for (auto& keyboardAxis : m_keyboardAxis)
	{
		keyboardAxis.second.currentValue = 0;
	}
}

void KeyboardInput::OnKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode,
	cocos2d::Event* pEvent)
{
	SetStateKeyState(false, keyCode);
	SetKeyboardAxisState(false, keyCode);

	// Action keys work once based on full press and release sequence
	SetActionKeyState(true, keyCode);
}

void KeyboardInput::OnKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode,
	cocos2d::Event* pEvent)
{
	SetStateKeyState(true, keyCode);
	SetKeyboardAxisState(true, keyCode);
}

void KeyboardInput::UpdateAxisKeyState(float deltaTime)
{
	// Gradually reach max axis value
	for (auto& pair : m_keyboardAxis)
	{
		KeyboardAxis& axis = pair.second;

		if (axis.fromIsPressed && !axis.toIsPressed)
		{
			IncreaseAxisCurValue(axis, axis.fromValue, deltaTime);
		}		
		else if (axis.toIsPressed && !axis.fromIsPressed)
		{
			IncreaseAxisCurValue(axis, axis.toValue, deltaTime);
		}
		else if (axis.fromIsPressed && axis.toIsPressed)
		{
			axis.currentValue = 0;
		}
	}
}

void KeyboardInput::SetActionKeyState(bool isActive, KeyCode keyCode)
{
	const int inputCode = static_cast<int>(keyCode);
	AInputDevice::SetActionButtonState(isActive, inputCode);
}

void KeyboardInput::SetStateKeyState(bool isPressed, KeyCode keyCode)
{
	const int inputCode = static_cast<int>(keyCode);
	AInputDevice::SetStateButtonState(isPressed, inputCode);
}

void KeyboardInput::SetKeyboardAxisState(bool bIsPressed, KeyCode keyCode)
{
	if (Utils::ContainsKey(m_keyCodeToAxisAction, keyCode))
	{
		const String& action = m_keyCodeToAxisAction[keyCode];
		KeyboardAxis& axis = m_keyboardAxis[action];
		if (axis.keyCodeFrom == keyCode)
		{			
			axis.fromIsPressed = bIsPressed;
		}
		else if (axis.keyCodeTo == keyCode)
		{
			axis.toIsPressed = bIsPressed;
		}
		
		if (!bIsPressed)
		{
			// When button is released axis value should be set to 0
			axis.currentValue = 0;
		}
	}
}

float KeyboardInput::GetAxisInput(const String& axisName) const
{
	float value = 0;
	if (HasAxisInput(axisName))
	{
		// Return the sum of all axis key values
		value = m_keyboardAxis.at(axisName).currentValue;
	}
	return value;
}

bool KeyboardInput::HasAxisInput(const String& axisName) const
{
	return Utils::ContainsKey(m_keyboardAxis, axisName);
}

void KeyboardInput::IncreaseAxisCurValue(KeyboardAxis & keyboardAxis, float value, float deltaTime)
{
	keyboardAxis.currentValue += Utils::GetSign(value) * deltaTime;

	// Make sure we stay in range
	if (keyboardAxis.fromValue > keyboardAxis.toValue)
	{
		Utils::ClampValue(keyboardAxis.currentValue,
			keyboardAxis.toValue, keyboardAxis.fromValue);
	}
	else
	{
		Utils::ClampValue(keyboardAxis.currentValue,
			keyboardAxis.fromValue, keyboardAxis.toValue);
	}
}


