//
//  ActionUtils.h
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/29.
//
//

#ifndef ActionUtils_hpp
#define ActionUtils_hpp

#include "cocos2d.h"

namespace ActionUtils {
    template<class... Args>
    void runActionSeq(cocos2d::Node* target, std::function<void()> endCallback, Args&&... args) {
        target->runAction(cocos2d::Sequence::create(args..., cocos2d::CallFunc::create(endCallback), nullptr));
    }
    
//    template<class... Args>
//    void runActionSeq(cocos2d::Node* target, Args&&... args) {
//        target->runAction(cocos2d::Sequence::create(args..., nullptr));
//    }
}

#endif /* ActionUtils_h */
