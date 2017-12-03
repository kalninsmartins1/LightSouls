#include "InputManager.h"

using namespace cocos2d;

InputManager* InputManager::getInstance()
{
	static InputManager inputManager;
	return &inputManager;
}

void InputManager::setInputContext(Node* pNode)
{
	// Set up input listeners
		// Create the listener objects	
	auto keyBoardListener = EventListenerKeyboard::create();
	auto controllerListener = EventListenerController::create();

		// Assign callbacks	
	keyBoardListener->onKeyReleased = CC_CALLBACK_1(InputManager::onKeyboardDown, this);
	//controllerListener->onAxisEvent = CC_CALLBACK_1
	//controllerListener->on
		// Add listeners to listening list	
	pNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardListener,
		pNode);	
}

void InputManager::onKeyboardDown(EventKeyboard::KeyCode keyCode) const
{

}

void InputManager::onControllerButtonDown(Controller* pController, int buttonCode, Event* pEvent) const
{
	
}

void InputManager::onControllerAxisEventDown(Controller* pController, int buttonCode, Event* pEvent) const
{
	
}