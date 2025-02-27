#ifndef __STAGE_H__
#define __STAGE_H__

#include "cocos2d.h"

class Stage : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int stageRef, unsigned int levelRef );
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Stage);
    
private:
    void GoToGamePlay( cocos2d::Ref *sender );
    void GoToStageSelect( cocos2d::Ref *sender );
    void GoToLevels1( cocos2d::Ref *sender );
    void GoToLevels2( cocos2d::Ref *sender );
    void GoToLevels3( cocos2d::Ref *sender );
};

#endif // __STAGE__H__
