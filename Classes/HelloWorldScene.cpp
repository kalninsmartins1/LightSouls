#include "HelloWorldScene.h"
#include "World/Entity/Player/Player.h"
#include "Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/CustomActions/CameraFallow.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// create the scene with physics enabled
	auto scene = createWithPhysics();

	auto layer = create();
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
	
	initWolrdLayer();
	initUILayer();
	
	// Init Input
	if(!GameInput::getInstance()->loadInputConfiguration("res/Configs/Input/Input.xml"))
	{
		// Halt the game when in debug mode
		CCASSERT(false, "HelloWorldScene: Failed to load input configuration !");
	}

	// Call update for this scene
	scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float deltaTime)
{	
	// Keep input events up to date
	GameInput::getInstance()->update(deltaTime);

	// Update AI
	AIAgentManager::getInstance()->update(deltaTime);
}

void HelloWorld::initWolrdLayer()
{
	Node* pWorldLayer = Node::create();

	// Init world
	World* pWorld = World::create("res/Configs/World/WorldConfig.xml");
	pWorldLayer->addChild(pWorld);

	// Init player
	m_pPlayer = Player::create("res/Configs/World/Player/Player.xml");
	pWorldLayer->addChild(m_pPlayer);

	// Init AI
	AIAgentManager* pAgentManger = AIAgentManager::getInstance();
	if (pAgentManger->init("res/Configs/World/AI/AIManager.xml"))
	{
		pAgentManger->setTargetEntity(m_pPlayer);
		pAgentManger->setWorldLayer(pWorldLayer);

		// Spawn agents in world
		pAgentManger->spawnAgent("WarriorAgent", Vec2(200, 200));
	}
	else
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize AI manager !");
	}

	// Set world camera mask
	pWorldLayer->setCameraMask(static_cast<unsigned short int>(
		CameraFlag::USER1));
	addChild(pWorldLayer);

	// Create world camera and set it to follow player
	Camera* pWorldCamera = Camera::create();
	pWorldCamera->setCameraFlag(CameraFlag::USER1);
	pWorldCamera->runAction(CameraFollow::create(m_pPlayer));
	addChild(pWorldCamera);
}

void HelloWorld::initUILayer()
{
	// Init UI
	Node* pUILayer = Node::create();
	Sprite* pScreenOverlay = Sprite::create("res/Graphics/UI/screenOverlay.png");
	const Vec2 scale = Utils::getScreenFillScale(pScreenOverlay->getContentSize());
	pScreenOverlay->setScale(scale.x, scale.y);
	pScreenOverlay->setAnchorPoint(Vec2::ZERO);

	pUILayer->addChild(pScreenOverlay);
	pUILayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));
	addChild(pUILayer);

	// Create UI camera
	Camera* pUICamera = Camera::create();
	pUICamera->setCameraFlag(CameraFlag::USER2);
	addChild(pUICamera);
}
