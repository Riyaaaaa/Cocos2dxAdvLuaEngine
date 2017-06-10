//
//  VisualNovelScene.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "VisualNovelScene.h"

using namespace cocos2d;

bool VisualNovelScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    
    
    return true;
}

void VisualNovelScene::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text:
            _talkText->setString(libspiral::any_cast<NovelContext>(context.second.getContext()).text);
            break;
        case ScriptFuncType::SetName:
            break;
        case ScriptFuncType::PlaceCharacter:
            break;
        default:
            break;
    }
}