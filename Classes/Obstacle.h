#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "cocos2d.h"

class Obstacle
{
public:
    Obstacle();
    
    void CreateObstacle( cocos2d::Layer *layer, cocos2d::Sprite *sprite );
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
};

#endif // __OBSTACLE_H__
