//
//  GradationSprite.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/15.
//
//

#include "GradationSprite.h"

USING_NS_CC;

GradationSprite* GradationSprite::create(std::string filename) {
    GradationSprite* ptr = new GradationSprite;
    
    if (ptr && ptr->init(filename)) {
        ptr->autorelease();
        return ptr;
    }
    
    delete ptr;
    return nullptr;
}

bool GradationSprite::init(std::string filename) {
    if (!Sprite::initWithFile(filename)) {
        return false;
    }
    
    _previousSprite = Sprite::create(filename);
    _previousSprite->setAnchorPoint(Vec2::ZERO);
    addChild(_previousSprite);
    _previousSprite->setVisible(false);
    
    return true;
}

void GradationSprite::setTexture(std::string filename, std::function<void()> callback) {
    _previousSprite->setVisible(true);
    //this->setOpacity(0);
    Sprite::setTexture(filename);
    
    this->runAction(FadeTo::create(0.5f, 255));
    _previousSprite->runAction(Sequence::create(FadeTo::create(0.5f, 0),
                                                CallFunc::create([this, filename, callback](){
        _previousSprite->setVisible(false);
        _previousSprite->setOpacity(255);
        _previousSprite->setTexture(filename);
        if (callback) {
            callback();
        }
    }),
                                                nullptr));
}
