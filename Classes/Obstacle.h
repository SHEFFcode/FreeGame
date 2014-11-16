#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"

class Obstacle
{
public:
    Obstacle();
    
    void CreateObstacle( cocos2d::Layer *layer, cocos2d::TMXLayer *mapLayer, unsigned int bitmask, unsigned int x);
    void EnableTiles( cocos2d::TMXLayer *mapLayer, unsigned int bitmask, unsigned int numTiles );
    void RemoveTiles( cocos2d::TMXLayer *mapLayer, unsigned int x );
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
};

#endif // __OBSTACLE_H__
