//
//  NovelTextUtils.h
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/18.
//
//

#ifndef NovelTextUtils_h
#define NovelTextUtils_h

#include "ui/CocosGUI.h"
#include "cocos2d.h"

namespace NovelTextUtils {
    void runCaption(cocos2d::ui::Text* text, float duration, std::function<void()> endCallback = nullptr);
    void clearTextAnimation(cocos2d::ui::Text* text);
}

#endif /* NovelTextUtils_h */
