//
//  CharacterStandView.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/15.
//
//

#include "CharacterStandView.h"

CharacterStandView* CharacterStandView::create(std::string filename) {
    CharacterStandView* ptr = new CharacterStandView;
    
    if (ptr && ptr->init(filename)) {
        ptr->autorelease();
        return ptr;
    }
    
    delete ptr;
    return nullptr;
}

bool CharacterStandView::init(std::string filename) {
    if (!GradationSprite::init(filename)) {
        return false;
    }
    
    return true;
}