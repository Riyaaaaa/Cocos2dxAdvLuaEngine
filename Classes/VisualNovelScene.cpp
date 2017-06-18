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
    
    _scene = CSLoader::getInstance()->createNode("VisualNovelScene.csb");
    addChild(_scene);
    
    _talkText = _scene->getChildByName("TalkWindow")->getChildByName<ui::Text*>("TalkText");
    _nameText = _scene->getChildByName("NameWindow")->getChildByName<ui::Text*>("NameText");
    
    _characterAnchors[0] = _scene->getChildByName("LeftAnchor");
    _characterAnchors[1] = _scene->getChildByName("CenterAnchor");
    _characterAnchors[2] = _scene->getChildByName("RightAnchor");
    
    _engine.setScriptHandler(CC_CALLBACK_1(VisualNovelScene::scriptHandler, this));
    
    return true;
}

void VisualNovelScene::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text:
            _talkText->setString(libspiral::any_cast<NovelContext>(context.second.getContext()).text);
            this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([this](){ _engine.progress(); }), NULL));
            break;
        case ScriptFuncType::SetName:
            _nameText->setString(libspiral::any_cast<NameContext>(context.second.getContext()).name);
            _engine.progress();
            break;
        case ScriptFuncType::PlaceCharacter:
            _engine.progress();
            break;
        case ScriptFuncType::End:
            break;
        default:
            _engine.progress();
            break;
    }
}

void VisualNovelScene::onEnter() {
    Scene::onEnter();
    _engine.run();
    _engine.progress();
}

