#include "KeyboardInput.h"
#include "Utils/Utils.h"

using namespace cocos2d;

KeyboardInput::KeyboardInput()
{
	if(!init())
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
}

void KeyboardInput::onKeyboardKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, 
	cocos2d::Event* pEvent)
{
}

void KeyboardInput::onKeyboardKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, 
	cocos2d::Event* pEvent)
{

}

void KeyboardInput::updateActionKeyState()
{
	// Make sure action keys are only active for one frame
	for(auto& pair : m_actionKeys)
	{
		ActionKey& key = pair.second;
		if(key.bIsActive)
		{
			key.bNeedsStateReset = true;
		}
		else if(key.bNeedsStateReset)
		{
			key.bIsActive = false;
			key.bNeedsStateReset = false;
		}
	}
}

bool KeyboardInput::hasAction(const std::string& action) const
{	
	bool bHasAction = false;
	if(Utils::containsKey(m_actionKeys, action))
	{
		bHasAction = m_actionKeys.at(action).bIsActive;
	}

	return bHasAction;
}

bool KeyboardInput::hasActionState(const std::string& action) const
{
	bool bHasActionState = false;
	if(Utils::containsKey(m_stateKeys, action))
	{
		bHasActionState = m_stateKeys.at(action).bIsCurrentlyPressed;
	}
	return bHasActionState;
}

float KeyboardInput::getAxisInput(const std::string& axisName) const
{
	float value = 0;
	if(Utils::containsKey(m_axisKeys, axisName))
	{
		value = m_axisKeys.at(axisName).currentValue;
	}
	return value;
}

bool KeyboardInput::hasAxisInput(const std::string& axisName) const
{
	return Utils::containsKey(m_axisKeys, axisName);
}
