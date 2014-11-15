#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"

class Obstacle : public cocos2d::Node
{
public:
    Obstacle();
    
    void CreateObstacle( cocos2d::Layer *layer, cocos2d::TMXLayer *mapLayer, unsigned int bitmask);
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    void EnableTilePhysics(float dt);
    
};

#endif // __OBSTACLE_H__
