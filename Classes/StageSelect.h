#ifndef __STAGE_SELECT_H__
#define __STAGE_SELECT_H__

#include "cocos2d.h"

class StageSelect : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene( unsigned int setRef );
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(StageSelect);
    
private:
    void Stage( cocos2d::Ref *sender );
    void GoToStageSelect( cocos2d::Ref *sender );
    void GoToMainMenu( cocos2d::Ref *sender );
    
};

#endif // __STAGE_SELECT_H__
