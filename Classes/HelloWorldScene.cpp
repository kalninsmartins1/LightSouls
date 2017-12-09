#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Entity/Player/Player.h"

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

	// Set input manager context	
	Player* pPlayer = Player::create("res/Configs/Player/Player.xml");	
	pRootNode->addChild(pPlayer);
	addChild(pRootNode);	
	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float deltaTime)
{
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->
		setDebugDrawMask(0xFFFFFF);
}
