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
        for (int x = 0; x < layerSize.width; x++)
        {
            auto sprite = mapLayer->getTileAt(Point(x, y));
            if(sprite)
            {
                auto spriteBody = PhysicsBody::createEdgeBox(sprite->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT, 1);
                spriteBody->setCollisionBitmask( bitmask );
                spriteBody->setContactTestBitmask( true );
                spriteBody->setEnable(false);
                sprite->setPhysicsBody(spriteBody);
            }
        }
        
        for (int x = 0; x < numTiles; x++)
        {
            auto sprite = mapLayer->getTileAt(Point(x, y));
            if(sprite)
            {
                sprite->getPhysicsBody()->setEnable(true);
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
            sprite->getPhysicsBody()->setEnable(true);
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
            sprite->getPhysicsBody()->setEnable(false);
        }
    }

}









