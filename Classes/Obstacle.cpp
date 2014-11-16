#include "Obstacle.h"
#include "Definitions.h"

USING_NS_CC;

Obstacle::Obstacle()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
}

void Obstacle::CreateObstacle( cocos2d::Layer *layer, cocos2d::TMXLayer *mapLayer, unsigned int bitmask, unsigned int numTiles)
{
    auto layerSize = mapLayer->getLayerSize();

    for (int y = 0; y < layerSize.height; y++)
    {
        for (int x = 0; x < numTiles; x++)
        {
            auto sprite = mapLayer->getTileAt(Point(x, y));
            if(sprite)
            {
                auto spriteBody = PhysicsBody::createBox(sprite->getContentSize()*.93, PHYSICSBODY_MATERIAL_DEFAULT);
                spriteBody->setCollisionBitmask( bitmask );
                spriteBody->setContactTestBitmask( true );
                spriteBody->setDynamic(false);
                spriteBody->setEnable(true);
                sprite->setPhysicsBody(spriteBody);
            }
        }
    }
}

void Obstacle:: EnableTiles( cocos2d::TMXLayer *mapLayer, unsigned int bitmask, unsigned int x )
{
    auto layerSize = mapLayer->getLayerSize();
    
    for (int y = 0; y < layerSize.height; y++)
    {
        auto sprite = mapLayer->getTileAt(Point(x, y));
        if(sprite)
        {
            auto spriteBody = PhysicsBody::createBox(sprite->getContentSize()*.93, PHYSICSBODY_MATERIAL_DEFAULT);
            spriteBody->setCollisionBitmask( bitmask );
            spriteBody->setContactTestBitmask( true );
            spriteBody->setDynamic(false);
            spriteBody->setEnable(true);
            sprite->setPhysicsBody(spriteBody);
        }
    }

}

void Obstacle::RemoveTiles( cocos2d::TMXLayer *mapLayer, unsigned int x )
{
    auto layerSize = mapLayer->getLayerSize();
    
    for (int y = 0; y < layerSize.height; y++)
    {
        auto sprite = mapLayer->getTileAt(Point(x, y));
        if(sprite)
        {
            sprite->setPhysicsBody(nullptr);
        }
    }

}









