#include "LoadingScreenScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Input/GameInput.h"
#include "ui/CocosGUI.h"
#include "Utils/Utils.h"
#include "GameConsts.h"
#include "ENextScene.h"
#include "GameOverScene.h"

USING_NS_CC;

LoadingScreenScene::LoadingScreenScene(LightSouls::ENextScene nextScene)
	: m_nextScene(nextScene)
{

}

Scene* LoadingScreenScene::CreateScene(LightSouls::ENextScene nextScene)
{
	LoadingScreenScene* scene = new (std::nothrow) LoadingScreenScene(nextScene);
	if (scene != nullptr && scene->init())
	{
		scene->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(scene);
	}

	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool LoadingScreenScene::init()
{
    //////////////////////////////
    // 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	float duration = 1.0f;
	LightSouls::Utils::StartTimerWithCallback(this, 
		CC_CALLBACK_0(LoadingScreenScene::SwitchToNextScene, this), duration);
	auto sprite = Sprite::create("res/Graphics/UI/loading.png");
	sprite->setScale(5.0f);
	sprite->setNormalizedPosition(Vec2(0.5f, 0.5f));
	sprite->runAction(RotateBy::create(duration, Vec3(0.0f, 0.0f, 450.0f * duration)));
	addChild(sprite);

    return true;
}

void LoadingScreenScene::SwitchToNextScene()
{	
	switch (m_nextScene)
	{		
		case LightSouls::ENextScene::MAIN_MENU:
			Director::getInstance()->replaceScene(MainMenuScene::CreateScene());
			break;

		case LightSouls::ENextScene::GAME:
			Director::getInstance()->replaceScene(GameScene::CreateScene());
			break;

		case LightSouls::ENextScene::GAME_OVER:
			Director::getInstance()->replaceScene(GameOverScene::CreateScene());
			break;	
	}		
}
