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

#include "ui/CocosGUI.h"

#include "cocos2d.h"

class VisualNovelScene : public cocos2d::Scene {
public:
    CREATE_FUNC(VisualNovelScene);
    bool init() override;
    
private:
    void onEnter() override;
    void scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context);
    //void onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
    
    float getDurationScriptFuncType(ScriptFuncType funcType);
    
    void enableTextCursor(bool enable);
    
    cocos2d::Node* _scene;
    
    cocos2d::ui::Text* _nameText;
    cocos2d::ui::Text* _talkText;
    
    cocos2d::Sprite* _bg;
    cocos2d::Node* _characterAnchors[3];
    
    cocos2d::Sprite* _cursor;
    
    NovelScriptEngine _engine;
    
    std::function<void()> _touchHandler;
    
    bool _isAuto;
    bool _waitProgress;    
};


#endif /* VisualNovelScene_hpp */
