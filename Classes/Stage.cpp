#include "Stage.h"
#include "StageSelect.h"
#include "GamePlay.h"
#include "Definitions.h"
#include "GamePlay.h"
#include "CCUserDefault.h"

USING_NS_CC;

unsigned int stageNum, levelSet;


Scene* Stage::createScene( unsigned int stageRef, unsigned int levelRef)
{
    stageNum = stageRef; // sets the stage number to stageNum
    levelSet = levelRef; // sets the level set for the stage
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Stage::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Stage::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    Vector<MenuItem*> menuArray(20);
    
    auto size = (visibleSize.width * 0.8 - 240) / 5;
    auto xStart = visibleSize.width * 0.1;
    auto yStart = visibleSize.height - (visibleSize.height * 0.075) - size;
    for (int i=0; i<5; i++){
        yStart = visibleSize.height - (visibleSize.height * 0.075) - size;
        if(i != 0 ){
            xStart = xStart + size + 60;
        }
        for (int j=0; j<3; j++)
        {
            if(j != 0)
            {
                yStart = yStart - size - 40;
            }
            
            __String *levelNum = __String::createWithFormat("%i", ((i + 1) + (j * 5)) + (levelSet * 15) );
            
            int levelInt = (((i + 1) + (j * 5)) + (levelSet * 15))-1;
            
            // stage unlocking code
            
            auto num = Label::createWithTTF(levelNum->getCString(), "Action Man.ttf", visibleSize.height * 0.05 );
            num->setColor(Color3B::WHITE);
            num->setPosition(Point( xStart + num->getContentSize().width / 2 + 5, yStart + size - num->getContentSize().height / 2 - 5));
            this->addChild(num,1);
            
            for (int s=0; s<10; s++) {
            maxLevelString = __String::createWithFormat("Max Level%i", stageNum);
            maxStarString = cocos2d::__String::createWithFormat("Max Star%i.%i",stageNum, levelInt+1);

            }
            
            
            unlockStage = std::vector <bool> (45);

                int maxLevel = UserDefault::getInstance()->getIntegerForKey(maxLevelString->getCString(), 1);
                for (int l=0; l<maxLevel; l++) {
                    unlockStage [l] = (true);
                    if (unlockStage.at(levelInt) == true){
                        level = Sprite::create("blue.jpg", Rect(xStart, yStart, size, size));
                        UserDefault::getInstance()->getIntegerForKey(maxStarString->getCString(),0);
                        maxStars = UserDefault::getInstance()->getIntegerForKey(maxStarString->getCString(),0);
                        star1 = cocos2d::Sprite::create("star.png");
                        star1->setColor(cocos2d::Color3B::BLACK);
                        star1->setScale(.01, .01);
                        this->addChild(star1,2);
                        star1->cocos2d::Node::setPosition(Point( xStart + num->getContentSize().width / 2 + 10, yStart + size - num->getContentSize().height / 2 - 100));
                        
                        star2 = cocos2d::Sprite::create("star.png");
                        star2->setColor(cocos2d::Color3B::BLACK);
                        star2->setScale(.01, .01);
                        this->addChild(star2,2);
                        star2->cocos2d::Node::setPosition(Point( xStart + num->getContentSize().width / 2 + 10 + 20, yStart + size - num->getContentSize().height / 2 - 100));
                        
                        star3 = cocos2d::Sprite::create("star.png");
                        star3->setColor(cocos2d::Color3B::BLACK);
                        star3->setScale(.01, .01);
                        this->addChild(star3,2);
                        star3->cocos2d::Node::setPosition(Point( xStart + num->getContentSize().width / 2 + 10 + 40, yStart + size - num->getContentSize().height / 2 - 100));
                        
                        if (maxStars == 3) {
                            star1->setColor(cocos2d::Color3B::YELLOW);
                            star2->setColor(cocos2d::Color3B::YELLOW);
                            star3->setColor(cocos2d::Color3B::YELLOW);
                        }
                        if (maxStars == 2) {
                            star1->setColor(cocos2d::Color3B::YELLOW);
                            star2->setColor(cocos2d::Color3B::YELLOW);
                        }
                        
                        if (maxStars == 1) {
                            star1->setColor(cocos2d::Color3B::YELLOW);
                        }

                    } else {
                        level = Sprite::create("red.jpg", Rect(xStart, yStart, size, size));
                    }

                }

        
            auto item = MenuItemSprite::create(level, level, level, CC_CALLBACK_1(Stage::GoToGamePlay, this));
            item->setPosition(Point(xStart + ( size / 2), yStart + ( size / 2)));
            item->setTag( ((i + 1) + (j * 5)) + (levelSet * 15) );
            menuArray.insert(i, item);

            
            

            

            }


        }
    
    auto main_menu = MenuItemImage::create("main_menu.png", "main_menu.png", "main_menu.png", CC_CALLBACK_1(Stage::GoToStageSelect, this));
    main_menu->setPosition(Point( visibleSize.width * 0.05 + ( main_menu->getContentSize().width / 2), visibleSize.height * 0.1));
    menuArray.insert(15, main_menu);
    
    if(levelSet == 0) {
        auto arrow = MenuItemImage::create("arrow_right.jpg", "arrow_right.jpg", "arrow_right.jpg", CC_CALLBACK_1(Stage::GoToLevels2, this));
        arrow->setPosition(Point(visibleSize.width - 10 - arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(16, arrow);
    } else if(levelSet == 1){
        auto arrow = MenuItemImage::create("arrow_left.jpg", "arrow_left.jpg", "arrow_left.jpg", CC_CALLBACK_1(Stage::GoToLevels1, this));
        arrow->setPosition(Point(10 + arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(16, arrow);
        auto arrowRight = MenuItemImage::create("arrow_right.jpg", "arrow_right.jpg", "arrow_right.jpg", CC_CALLBACK_1(Stage::GoToLevels3, this));
        arrowRight->setPosition(Point(visibleSize.width - 10 - arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(17, arrowRight);
    } else if(levelSet == 2){
        auto arrow = MenuItemImage::create("arrow_left.jpg", "arrow_left.jpg", "arrow_left.jpg", CC_CALLBACK_1(Stage::GoToLevels2, this));
        arrow->setPosition(Point(10 + arrow->getContentSize().width / 2, visibleSize.height / 2));
        menuArray.insert(16, arrow);
    }
    
    // Menu
    auto menu = Menu::createWithArray(menuArray);
    menu->setPosition(Point::ZERO);
    menu->setZOrder(0);
    this->addChild(menu);
    
//    level->setZOrder(0);

    
    
    return true;
}

void Stage::GoToGamePlay( cocos2d::Ref *sender )
{
    MenuItem* pMenuItem = (MenuItem *)(sender);
    
    int levelSet = (int)pMenuItem->getTag();
    
    auto scene = GamePlay::createScene(stageNum, levelSet); // stage num, level num
    
    Director::getInstance()->replaceScene(scene);
}

void Stage::GoToStageSelect(cocos2d::Ref *sender)
{
    auto scene = StageSelect::createScene(0);
    
    Director::getInstance()->replaceScene(scene);
}

void Stage::GoToLevels1(cocos2d::Ref *sender)
{
    levelSet = 0;
    
    auto scene = Stage::createScene(stageNum, levelSet);
    
    Director::getInstance()->replaceScene(scene);
}

void Stage::GoToLevels2(cocos2d::Ref *sender)
{
    levelSet = 1;
    
    auto scene = Stage::createScene(stageNum, levelSet);
    
    Director::getInstance()->replaceScene(scene);
}

void Stage::GoToLevels3(cocos2d::Ref *sender)
{
    levelSet = 2;
    
    auto scene = Stage::createScene(stageNum, levelSet);
    
    Director::getInstance()->replaceScene(scene);
}


















