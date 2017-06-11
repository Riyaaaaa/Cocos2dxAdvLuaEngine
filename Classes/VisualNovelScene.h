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
    void scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context);
    
    cocos2d::Node* _scene;
    
    cocos2d::ui::Text* _nameText;
    cocos2d::ui::Text* _talkText;
    
    NovelScriptEngine _engine;
};


#endif /* VisualNovelScene_hpp */
