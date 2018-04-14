#include "HelloWorldScene.h"
#include "World/Entity/Player/Player.h"
#include "Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/CustomActions/CameraFallow.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "UI/InGameIndicators/ProgressBar.h"
#include "Events/HealthChangedEventData.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// create the scene with physics enabled
	auto scene = createWithPhysics();

	Node* layer = create();
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
	
	InitWolrdLayer();
	InitUILayer();
	
	// Init Input
	if(!GameInput::GetInstance()->LoadInputConfiguration("res/Configs/Input/Input.xml"))
	{
		// Halt the game when in debug mode
		CCASSERT(false, "HelloWorldScene: Failed to load input configuration !");
	}

	// Init physics manager
	if(!PhysicsManager::GetInstance()->Init(this))
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize PhysicsManager !");
	}

	// Call update for this scene
	scheduleUpdate(); 
    
    return true;
}

void HelloWorld::update(float deltaTime)
{	
	// Keep input events up to date
	GameInput::GetInstance()->Update(deltaTime);

	// Update AI
	AIAgentManager::GetInstance()->Update(deltaTime);	

	PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	if(world != nullptr)
	{
		world->setDebugDrawMask(0xFFFFFF);
		world->setDebugDrawCameraMask(CameraFlag::USER1);
	}

	m_healthBar->Update(deltaTime);
}

void HelloWorld::InitWolrdLayer()
{
	Node* worldLayer = Node::create();

	// Init world
	World* pWorld = World::Create("res/Configs/World/WorldConfig.xml");
	worldLayer->addChild(pWorld);

	// Init player
	m_player = Player::Create("res/Configs/World/Player/Player.xml");
	worldLayer->addChild(m_player);
	
	getEventDispatcher()->addCustomEventListener(Player::GetOnHealthChangedEvent(),
		CC_CALLBACK_1(HelloWorld::OnPlayerHealthChanged, this));

	// Init AI
	AIAgentManager* agentManager = AIAgentManager::GetInstance();
	if (agentManager->Init("res/Configs/World/AI/AIManager.xml"))
	{
		agentManager->SetTargetEntity(m_player);
		agentManager->SetWorldLayer(worldLayer);

		// Spawn agents in world
		agentManager->SpawnAgent("WarriorAgent", Vec2(200, 200));
	}
	else
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize AI manager !");
	}

	// Set world camera mask
	worldLayer->setCameraMask(static_cast<unsigned short int>(
		CameraFlag::USER1));
	addChild(worldLayer);

	// Create world camera and set it to follow player
	Camera* worldCamera = Camera::create();
	worldCamera->setCameraFlag(CameraFlag::USER1);
	worldCamera->runAction(CameraFollow::Create(m_player));
	addChild(worldCamera);	
}

void HelloWorld::InitUILayer()
{
	// Init UI
	Node* uiLayer = Node::create();
	uiLayer->setContentSize(Utils::GetScreenSize());

	Sprite* screenOverlay = Sprite::create("res/Graphics/UI/screenOverlay.png");
	const Vec2 scale = Utils::GetScreenFillScale(screenOverlay->getContentSize());
	screenOverlay->setScale(scale.x, scale.y);
	screenOverlay->setAnchorPoint(Vec2::ZERO);

	m_healthBar = ProgressBar::Create("res/Configs/UI/InGameIndicators/HealthBar.xml");
	if (m_healthBar == nullptr)
	{
		CCLOG("HelloWorldScene: Failed to initialize health bar !");
	}
	
	// TODO: Add stamina bar
	uiLayer->addChild(screenOverlay);
	uiLayer->addChild(m_healthBar);
	uiLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));
	addChild(uiLayer);

	// Create UI camera
	Camera* pUICamera = Camera::create();
	pUICamera->setCameraFlag(CameraFlag::USER2);
	addChild(pUICamera);
}

void HelloWorld::OnPlayerHealthChanged(EventCustom* eventData)
{
	auto healthData = static_cast<HealthChangedEventData*>(eventData->getUserData());
	
	if (healthData != nullptr)
	{
		m_healthBar->SetCurrentValue(healthData->GetHealthPercentage());
	}	
}

