#include "MainMenu.h"
#include "StageSelect.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Menu Items
    auto play = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(MainMenu::Play, this));
    play->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    auto miniGames = MenuItemImage::create("miniGames.png", "miniGames.png", CC_CALLBACK_1(MainMenu::MiniGames, this));
    miniGames->setPosition(Point(visibleSize.width / 2 + origin.x, (visibleSize.height / 2) - 105 + origin.y));
    
    auto options = MenuItemImage::create("options.png", "options.png", CC_CALLBACK_1(MainMenu::Options, this));
    options->setPosition(Point(visibleSize.width / 2 + origin.x, (visibleSize.height / 2) - 210 + origin.y));
    
    // Menu
    auto menu = Menu::create(play, miniGames, options, NULL);
    menu->setPosition(Point::ZERO);
    
    this->addChild(menu);
    
    return true;
}

void MainMenu::Play(cocos2d::Ref *sender)
{
    auto scene = StageSelect::createScene(0);
    
    Director::getInstance()->replaceScene(scene);
}

void MainMenu::MiniGames(cocos2d::Ref *sender)
{
    
}

void MainMenu::Options(cocos2d::Ref *sender)
{
    
}








