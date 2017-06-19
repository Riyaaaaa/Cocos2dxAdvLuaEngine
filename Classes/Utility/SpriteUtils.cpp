//
//  SpriteUtils.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/19.
//
//

#include "SpriteUtils.h"

USING_NS_CC;

void SpriteUtils::fadeIn(cocos2d::Sprite* sprite, float duration, std::function<void()> endCallback) {
    sprite->setOpacity(0);
    sprite->runAction(Sequence::create(FadeIn::create(duration), CallFunc::create(endCallback), nullptr));
}