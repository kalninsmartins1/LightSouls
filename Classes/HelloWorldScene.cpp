#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Entity/Player/Player.h"
#include "Controlls/InputManager.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
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
	InputManager::getInstance()->setInputContext(this);
	m_pPlayer = Player::create("Configs/Player/Player.xml");	
	pRootNode->addChild(m_pPlayer);
	addChild(pRootNode);	
	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float deltaTime)
{
	m_pPlayer->update(deltaTime);
}
