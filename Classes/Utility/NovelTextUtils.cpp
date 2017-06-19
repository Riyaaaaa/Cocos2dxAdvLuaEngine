//
//  NovelTextUtils.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/18.
//
//

#include "NovelTextUtils.h"

#include "MaybeCall.hpp"

USING_NS_CC;

void NovelTextUtils::runCaption(ui::Text *text, float duration, std::function<void()> endCallback) {
    ssize_t textLength = text->getStringLength();
    auto label = dynamic_cast<Label*>(text->getVirtualRenderer());
    
    for (int i = 0; i < textLength; i++) {
        Sprite *sp = label->getLetter(i);
        if (sp) {
            sp->setOpacity(0);
            text->runAction(Sequence::create(
                                           DelayTime::create(duration * i),
                                           CallFunc::create([sp, i, textLength, endCallback](){
                sp->setOpacity(255);
                if (i == textLength - 1) {
                    libspiral::maybeCall(endCallback);
                }
                //SimpleAudioEngine::getInstance()->playEffect(SOUND_FILE_TEXT_SE);
            }),
                                           NULL
                                           ));
        }
    }
}

void NovelTextUtils::clearTextAnimation(cocos2d::ui::Text* text) {
    ssize_t textLength = text->getStringLength();
    auto label = dynamic_cast<Label*>(text->getVirtualRenderer());
    for (int i = 0; i < textLength; i++) {
        label->getLetter(i)->setOpacity(255);
    }
    text->stopAllActions();
}
