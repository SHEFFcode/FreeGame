#include "SplashScreen.h"
#include "Definitions.h"
#include "MainMenu.h"

USING_NS_CC;

Scene* SplashScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("splash_screen.jpg");
    background->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background);
    
    this->scheduleOnce(schedule_selector(SplashScreen::GoToMainMenu), DISPLAY_TIME_SPLASH_SCREEN);
    
    return true;
}

void SplashScreen::GoToMainMenu(float dt)
{
    auto scene = MainMenu::createScene();
    Director::getInstance()->replaceScene(scene);
}





