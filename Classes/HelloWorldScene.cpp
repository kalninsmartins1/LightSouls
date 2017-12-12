#include "HelloWorldScene.h"
#include "Entity/Player/Player.h"
#include "Camera/CameraController.h"

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
    if ( !Scene::init() )
    {
        return false;
    }
	Node* pRootNode = Node::create();

	// Init player	
	m_pPlayer = Player::create("res/Configs/Player/Player.xml");	
	pRootNode->addChild(m_pPlayer);
	addChild(pRootNode);	

	m_pCameraController = CameraController::create();
	if(!m_pCameraController->init())
	{
		cocos2d::log("HelloWorldScene: Failed to initialize camera controller !");
	}
	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float deltaTime)
{
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->
		setDebugDrawMask(0xFFFFFF);

	m_pCameraController->moveCameraTo(convertToWorldSpace(
		m_pPlayer->getPosition()), 2);
}
