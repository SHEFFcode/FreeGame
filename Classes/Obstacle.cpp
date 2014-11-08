#include "Obstacle.h"
#include "Definitions.h"

USING_NS_CC;

Obstacle::Obstacle()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}

void Obstacle::CreateObstacle( cocos2d::Layer *layer, cocos2d::Sprite *sprite)
{
    
}

// set up obstacle layer movement

