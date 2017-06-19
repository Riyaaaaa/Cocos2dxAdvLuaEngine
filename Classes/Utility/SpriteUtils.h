//
//  SpriteUtils.hpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/19.
//
//

#ifndef SpriteUtils_h
#define SpriteUtils_h

#include "cocos2d.h"

namespace SpriteUtils {
    void fadeIn(cocos2d::Sprite* sprite, float duration, std::function<void()> endCallback);
}

#endif /* SpriteUtils_hpp */
