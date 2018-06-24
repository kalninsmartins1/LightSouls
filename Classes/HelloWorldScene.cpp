#include "HelloWorldScene.h"
#include "World/Entity/Player/Player.h"
#include "Input/GameInput.h"
#include "World/World.h"
#include "World/Entity/CustomActions/CameraFallow.h"
#include "Utils/Utils.h"
#include "World/Entity/AI/AIAgentManager.h"
#include "World/Physics/PhysicsManager.h"
#include "UI/InGameIndicators/ProgressBar.h"
#include "Events/ProgressBarChangedEventData.h"
#include "Camera/Camera.h"

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

	#if LIGHTSOULS_RELEASE
	Director::getInstance()->setDisplayStats(false);
	#endif

	InitWolrdLayer();
	InitUILayer();
	
	// Init Input
	if(!LightSouls::GameInput::GetInstance()->LoadInputConfiguration("res/Configs/Input/Input.xml"))
	{
		// Halt the game when in debug mode
		CCASSERT(false, "HelloWorldScene: Failed to load input configuration !");
	}

	// Init physics manager
	if(!LightSouls::PhysicsManager::GetInstance()->Init(this))
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
	auto gameInput = LightSouls::GameInput::GetInstance();
	gameInput->Update(deltaTime);
	if (gameInput->HasAction("ExitGame"))
	{
		Director::getInstance()->end();
		return;
	}

	// Update AI
	LightSouls::AIAgentManager::GetInstance()->Update(deltaTime);	

	// Debug physics 
// 	PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
// 	if(world != nullptr)
// 	{
// 		world->setDebugDrawMask(0xFFFFFF);
// 		world->setDebugDrawCameraMask(CameraFlag::USER1);
// 	}

	if (m_healthBar != nullptr)
	{
		m_healthBar->Update(deltaTime);
	}

	if (m_staminaBar != nullptr)
	{
		m_staminaBar->Update(deltaTime);
	}
}

void HelloWorld::InitWolrdLayer()
{
	Node* worldLayer = Node::create();

	// Init world
	LightSouls::World* pWorld = LightSouls::World::Create("res/Configs/World/WorldConfig.xml");
	worldLayer->addChild(pWorld);

	// Init player
	m_player = LightSouls::Player::Create("res/Configs/World/Player/Player.xml");
	worldLayer->addChild(m_player);
	
	getEventDispatcher()->addCustomEventListener(LightSouls::Player::GetEventOnHealthChanged(),
		CC_CALLBACK_1(HelloWorld::OnPlayerHealthChanged, this));
	getEventDispatcher()->addCustomEventListener(LightSouls::Player::GetEventOnStaminaChanged(),
		CC_CALLBACK_1(HelloWorld::OnPlayerStaminaChanged, this));

	// Init AI
	LightSouls::AIAgentManager* agentManager = LightSouls::AIAgentManager::GetInstance();
	if (agentManager->Init(worldLayer, "res/Configs/World/AI/AIManager.xml"))
	{
		agentManager->SetTargetEntity(m_player);
	}
	else
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize AI manager !");
	}

	// Set world camera mask
	worldLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER1));
	addChild(worldLayer);

	// Create world camera and set it to follow player
	LightSouls::Camera* worldCamera = LightSouls::Camera::Create("res/Configs/Camera/Camera.xml");
	worldCamera->setCameraFlag(CameraFlag::USER1);
	worldCamera->runAction(LightSouls::CameraFollow::Create(m_player));
	addChild(worldCamera);	
}

void HelloWorld::InitUILayer()
{
	// Init UI
	Node* uiLayer = Node::create();
	uiLayer->setContentSize(LightSouls::Utils::GetScreenSize());

	Sprite* screenOverlay = Sprite::create("res/Graphics/UI/screenOverlay.png");
	const Vec2 scale = LightSouls::Utils::GetScreenFillScale(screenOverlay->getContentSize());
	screenOverlay->setScale(scale.x, scale.y);
	screenOverlay->setAnchorPoint(Vec2::ZERO);

	m_healthBar = LightSouls::ProgressBar::Create("res/Configs/UI/InGameIndicators/HealthBar.xml");
	if (m_healthBar == nullptr)
	{
		CCLOG("HelloWorldScene: Failed to initialize health bar !");
	}

	m_staminaBar = LightSouls::ProgressBar::Create("res/Configs/UI/InGameIndicators/StaminaBar.xml");
	if (m_staminaBar == nullptr)
	{
		CCLOG("HelloWorldScene: Failed to initialize stamina bar!");
	}
		
	uiLayer->addChild(screenOverlay);
	uiLayer->addChild(m_healthBar);
	uiLayer->addChild(m_staminaBar);
	uiLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));
	addChild(uiLayer);

	// Create UI camera
	Camera* pUICamera = Camera::create();
	pUICamera->setCameraFlag(CameraFlag::USER2);
	addChild(pUICamera);
}

void HelloWorld::OnPlayerHealthChanged(EventCustom* eventData)
{
	auto healthData = static_cast<LightSouls::ProgressBarChangedEventData*>(eventData->getUserData());
	
	if (healthData != nullptr && m_healthBar != nullptr)
	{
		m_healthBar->SetCurrentValue(healthData->GetPercentage());
	}	
}

void HelloWorld::OnPlayerStaminaChanged(cocos2d::EventCustom* eventData)
{
	auto staminaData = static_cast<LightSouls::ProgressBarChangedEventData*>(eventData->getUserData());

	if (staminaData != nullptr && m_staminaBar != nullptr)
	{
		m_staminaBar->SetCurrentValue(staminaData->GetPercentage());
	}
}