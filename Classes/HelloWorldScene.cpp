#include "HelloWorldScene.h"
#include "Entity/Player/Player.h"
#include "Camera/CameraController.h"
#include "Input/GameInput.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// create the scene with physics enabled
	auto scene = Scene::createWithPhysics();

	auto layer = HelloWorld::create();
	scene->addChild(layer);

	return scene;    
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Node* pRootNode = Node::create();

	// Init player
	m_pPlayer = Player::create("res/Configs/Player/Player.xml");
	pRootNode->addChild(m_pPlayer);
	addChild(pRootNode);

	m_pCameraController = CameraController::create();
	if(!GameInput::getInstance()->loadInputConfiguration("res/Configs/Input/Input.xml"))
	{
		// Halt the game when in debug mode
		CCASSERT(false, "HelloWorldScene: Failed to load input configuration !");
	}	

	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float deltaTime)
{
	m_pCameraController->moveCameraTo(convertToWorldSpace(
		m_pPlayer->getPosition()), 2);

	// Keep input events up to date
	GameInput::getInstance()->update(deltaTime);
}
