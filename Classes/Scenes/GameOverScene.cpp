#include "GameOverScene.h"
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

Scene* GameOverScene::CreateScene()
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
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}


	// Call update for this scene
	scheduleUpdate(); 
    
    return true;
}

void GameOverScene::update(float deltaTime)
{	
	
}