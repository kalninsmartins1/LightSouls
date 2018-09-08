#include "GameScene.h"
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
#include "LoadingScreenScene.h"
#include "ENextScene.h"
#include "ScoringSystem/ScoringSystem.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

LightSouls::PhysicsManager* GameScene::s_physicsManager = nullptr;
LightSouls::GameInput* GameScene::s_gameInput = nullptr;

GameScene::GameScene()
	: m_player(nullptr)
	, m_healthBar(nullptr)
	, m_staminaBar(nullptr)
	, m_scoreText(nullptr)
	, m_infoPlayerText(nullptr)
	, m_physicsDebugEnabled(false)
{
	// Reset player score upon new game
	LightSouls::ScoringSystem::GetInstance()->Reset();
}

GameScene::~GameScene()
{	
	LightSouls::AIAgentManager::GetInstance()->Cleanup();
	s_physicsManager->release();
	s_physicsManager = nullptr;
	s_gameInput->release();
	s_gameInput = nullptr;
}

LightSouls::PhysicsManager* GameScene::GetPhysicsManager()
{
	return s_physicsManager;
}

LightSouls::GameInput* GameScene::GetGameInput()
{
	return s_gameInput;
}

Scene* GameScene::CreateScene()
{
	// Create the scene with physics enabled
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
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	setCascadeOpacityEnabled(true);

	// Init physics manager
	s_physicsManager = LightSouls::PhysicsManager::Create(this);
	if (s_physicsManager == nullptr)
	{
		CCASSERT(false, "HelloWorldScene: Failed to initialize PhysicsManager !");
	}

	InitWolrdLayer();
	InitUILayer();

	// Init Input
	s_gameInput = LightSouls::GameInput::Create("res/Configs/Input/Input.xml");
	if (s_gameInput == nullptr)
	{
		// Halt the game when in debug mode
		CCASSERT(false, "HelloWorldScene: Failed to load input configuration !");
	}

	// Call update for this scene
	scheduleUpdate();
    
    return true;
}

void GameScene::update(float deltaTime)
{	
	// Keep input events up to date
	s_gameInput->Update(deltaTime);
	if (s_gameInput->HasAction("ExitGame"))
	{
		Director::getInstance()->end();
		return;
	}

	// Update AI
	LightSouls::AIAgentManager::GetInstance()->Update(deltaTime);	

#if !defined(LIGHTSOULS_RELEASE)
	if (s_gameInput->HasAction("ReloadGame"))
	{
		removeAllChildren();
		LightSouls::AIAgentManager::GetInstance()->Cleanup();
		InitWolrdLayer();
		InitUILayer();
	}
	
	ProcessDebugPhysicsDraw();
#endif

	if (m_healthBar != nullptr)
	{
		m_healthBar->Update(deltaTime);
	}

	if (m_staminaBar != nullptr)
	{
		m_staminaBar->Update(deltaTime);
	}
}

void GameScene::InitWolrdLayer()
{
	Node* worldLayer = Node::create();

	// Init world
	LightSouls::World* pWorld = LightSouls::World::Create("res/Configs/World/WorldConfig.xml");
	worldLayer->addChild(pWorld);

	// Init player
	m_player = LightSouls::Player::Create("res/Configs/World/Player/Player.xml");
	worldLayer->addChild(m_player);
	
	EventDispatcher* eventDispatcher = getEventDispatcher();
	eventDispatcher->addCustomEventListener(LightSouls::Player::GetEventOnHealthChanged(),
		CC_CALLBACK_1(GameScene::OnPlayerHealthChanged, this));
	eventDispatcher->addCustomEventListener(LightSouls::Player::GetEventOnStaminaChanged(),
		CC_CALLBACK_1(GameScene::OnPlayerStaminaChanged, this));
	eventDispatcher->addCustomEventListener(LightSouls::AIAgent::GetEventAgentDestroyed(),
		CC_CALLBACK_1(GameScene::OnAgentDestroyed, this));

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
	worldLayer->setCascadeOpacityEnabled(true);
	addChild(worldLayer);

	// Create world camera and set it to follow player
	LightSouls::Camera* worldCamera = LightSouls::Camera::Create("res/Configs/Camera/Camera.xml");
	worldCamera->setCameraFlag(CameraFlag::USER1);
	worldCamera->runAction(LightSouls::CameraFollow::Create(m_player));	
	addChild(worldCamera);	
}

void GameScene::InitUILayer()
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

	m_scoreText = ui::Text::create(StringUtils::format("Score: %d", LightSouls::ScoringSystem::GetInstance()->GetScore()),
		"Arial", 100);
	m_scoreText->setNormalizedPosition(Vec2(0.5f, 0.82f)); 

	m_infoPlayerText = ui::Text::create("Died !", "Arial", 100);
	m_infoPlayerText->setTextColor(Color4B::RED);
	m_infoPlayerText->setVisible(false);
	Node* infoContainer = Node::create();
	infoContainer->addChild(m_infoPlayerText);
	infoContainer->setNormalizedPosition(Vec2(0.5f, 0.65f));
	infoContainer->setCascadeOpacityEnabled(false);

	uiLayer->addChild(screenOverlay);
	uiLayer->addChild(m_healthBar);
	uiLayer->addChild(m_staminaBar);
	uiLayer->addChild(m_scoreText);
	uiLayer->addChild(infoContainer);
	uiLayer->setCameraMask(static_cast<unsigned short int>(CameraFlag::USER2));
	uiLayer->setCascadeOpacityEnabled(true);
	addChild(uiLayer);

	// Create UI camera
	Camera* uiCamera = Camera::create();
	uiCamera->setCameraFlag(CameraFlag::USER2);
	addChild(uiCamera);
}

void GameScene::OnPlayerHealthChanged(EventCustom* eventData)
{
	auto healthData = static_cast<LightSouls::ProgressBarChangedEventData*>(eventData->getUserData());
	
	if (healthData != nullptr && m_healthBar != nullptr)
	{
		m_healthBar->SetCurrentValue(healthData->GetPercentage());
		if (healthData->GetNewValue() <= 0)
		{
			m_healthBar->MultiplyAnimationSpeed(2.0f);
			m_infoPlayerText->setVisible(true);

			float toNextSceneTime = 2.0f;
			runAction(FadeOut::create(toNextSceneTime));
			LightSouls::Utils::StartTimerWithCallback(this,
				CC_CALLBACK_0(GameScene::SwitchToGameOverScene, this),
				toNextSceneTime);
		}
	}
}

void GameScene::OnPlayerStaminaChanged(cocos2d::EventCustom* eventData)
{
	auto staminaData = static_cast<LightSouls::ProgressBarChangedEventData*>(eventData->getUserData());

	if (staminaData != nullptr && m_staminaBar != nullptr)
	{
		m_staminaBar->SetCurrentValue(staminaData->GetPercentage());
	}
}

void GameScene::OnAgentDestroyed(cocos2d::EventCustom* eventData)
{
	using namespace LightSouls;
	ScoringSystem* scoringSystem = ScoringSystem::GetInstance();
	scoringSystem->IncreaseScore();
	m_scoreText->setString(StringUtils::format("Score: %d", scoringSystem->GetScore()));
}

void GameScene::SwitchToGameOverScene()
{
	getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(LoadingScreenScene::CreateScene(LightSouls::ENextScene::GAME_OVER));
}

void GameScene::ProcessDebugPhysicsDraw()
{
	if (s_gameInput->HasAction("DebugPhysics"))
	{
		m_physicsDebugEnabled = !m_physicsDebugEnabled;
	}
	
	PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	if (world != nullptr)
	{
		if (m_physicsDebugEnabled)
		{
			world->setDebugDrawMask(0xFFFFFF);
			world->setDebugDrawCameraMask(CameraFlag::USER1);
		}
		else
		{
			world->setDebugDrawMask(0);
			world->setDebugDrawCameraMask(CameraFlag::USER8);
		}
	}	
}
