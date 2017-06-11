//
//  VisualNovelScene.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "VisualNovelScene.h"

#include "NovelScriptContexts/NovelScriptContext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d;

bool VisualNovelScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    
    _scene = CSLoader::getInstance()->createNode("MainScene.csb");
    
    return true;
}

void VisualNovelScene::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text:
            _talkText->setString(libspiral::any_cast<NovelContext>(context.second.getContext()).text);
            break;
        case ScriptFuncType::SetName:
            _nameText->setString(libspiral::any_cast<NameContext>(context.second.getContext()).name);
            break;
        case ScriptFuncType::PlaceCharacter:
            break;
        default:
            break;
    }
}