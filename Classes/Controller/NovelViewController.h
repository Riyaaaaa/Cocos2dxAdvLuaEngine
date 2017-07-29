//
//  NovelViewController.hpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/08.
//
//

#ifndef NovelViewController_h
#define NovelViewController_h

#include "../View/VisualNovelScene.h"
#include "../NovelScriptContexts/ScriptFuncType.h"

class NovelViewController {
public:
    NovelViewController();
    
    void runScene(std::string filename, int transitType = 0);
    void transitScene(std::string filename);
    
private:
    void scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context);
    float getDurationScriptFuncType(ScriptFuncType funcType);
    
    void enableTextCursor(bool enable);
    
    void progress();
    
    std::function<void()> _touchHandler;
    
    NovelScriptEngine _engine;
    VisualNovelScene* _scene;
    
    cocos2d::Node* target;
    
    CharacterContext _placedCharacters[3];
    
    std::string _luaFileName;
    
    bool _isAuto;
    bool _waitProgress;
    bool _progressLock;
};

#endif /* NovelViewController_hpp */
