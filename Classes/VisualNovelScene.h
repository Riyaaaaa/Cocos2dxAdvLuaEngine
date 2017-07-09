//
//  VisualNovelScene.h
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#ifndef VisualNovelScene_h
#define VisualNovelScene_h

#include "NovelScriptEngine.h"
#include "View/ViewEventType.h"

#include "ui/CocosGUI.h"

#include "cocos2d.h"

class VisualNovelScene : public cocos2d::Scene {
public:
    static VisualNovelScene* createScene();
    bool init() override;
    
private:
    void onEnterTransitionDidFinish() override;
    
    CC_SYNTHESIZE(std::function<void(ViewEventType)>, _eventCallback, EventCallBack);
    
    CC_SYNTHESIZE(std::function<void(cocos2d::Touch*, cocos2d::Event*)>, _onTouchBegan, TouchCallback);
    
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, _nameText, NameText);
    CC_SYNTHESIZE_READONLY(cocos2d::ui::Text*, _talkText, TalkText);
    
    CC_SYNTHESIZE(cocos2d::Sprite*, _bg, Bg);
    CC_SYNTHESIZE(cocos2d::Sprite*, _cursor, Cursor);
    
    cocos2d::Node* getCharacterAnchor(int index) { return _characterAnchors[index]; }
    
    cocos2d::Node* _scene;
    
    cocos2d::Node* _characterAnchors[3];
};


#endif /* VisualNovelScene_hpp */
