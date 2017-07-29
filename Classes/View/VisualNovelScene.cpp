//
//  VisualNovelScene.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "VisualNovelScene.h"

#include "../NovelScriptContexts/NovelScriptContext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "../Utility/NovelTextUtils.h"
#include "../Utility/SpriteUtils.h"

using namespace cocos2d;

VisualNovelScene* VisualNovelScene::createScene() {
    VisualNovelScene* scene = new VisualNovelScene;
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    
    delete scene;
    return nullptr;
}

bool VisualNovelScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    _scene = CSLoader::getInstance()->createNode("VisualNovelScene.csb");
    addChild(_scene);
    
    _bg = _scene->getChildByName<Sprite*>("bg");
    
    _talkText = _scene->getChildByName("TalkWindow")->getChildByName<ui::Text*>("TalkText");
    _talkText->setString("");
    
    _nameText = _scene->getChildByName("NameWindow")->getChildByName<ui::Text*>("NameText");
    _nameText->setString("");
    
    _characterAnchors[0] = _scene->getChildByName("LeftAnchor");
    _characterAnchors[1] = _scene->getChildByName("CenterAnchor");
    _characterAnchors[2] = _scene->getChildByName("RightAnchor");
    
    _cursor = _scene->getChildByName("TalkWindow")->getChildByName<Sprite*>("Cursor");
    _cursor->setVisible(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* t, Event* e){
        if (_onTouchBegan) {
            _onTouchBegan(t, e);
        }
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void VisualNovelScene::onEnterTransitionDidFinish() {
    Scene::onEnterTransitionDidFinish();
    this->runAction(CallFunc::create([this](){ _eventCallback(ViewEventType::Enter); }));
}

