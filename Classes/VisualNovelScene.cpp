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

#include "Utility/NovelTextUtils.h"

using namespace cocos2d;

bool VisualNovelScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    _scene = CSLoader::getInstance()->createNode("VisualNovelScene.csb");
    addChild(_scene);
    
    _bg = _scene->getChildByName<Sprite*>("bg");
    
    _talkText = _scene->getChildByName("TalkWindow")->getChildByName<ui::Text*>("TalkText");
    _nameText = _scene->getChildByName("NameWindow")->getChildByName<ui::Text*>("NameText");
    
    _characterAnchors[0] = _scene->getChildByName("LeftAnchor");
    _characterAnchors[1] = _scene->getChildByName("CenterAnchor");
    _characterAnchors[2] = _scene->getChildByName("RightAnchor");
    
    _cursor = _scene->getChildByName("TalkWindow")->getChildByName<Sprite*>("Cursor");
    
    _engine.setScriptHandler(CC_CALLBACK_1(VisualNovelScene::scriptHandler, this));
    
    _isAuto = false;
    _waitProgress = false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch*, Event*){
        if (_waitProgress) {
            _engine.progress();
            _waitProgress = false;
        }
        
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void VisualNovelScene::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text:
            _talkText->setString(libspiral::any_cast<NovelContext>(context.second.getContext()).text);
            NovelTextUtils::runCaption(_talkText, 0.1, [this](){
                if (_isAuto) {
                    _engine.progress();
                } else {
                    _cursor->setVisible(true);
                    auto act = RepeatForever::create(Sequence::create(MoveBy::create(0.3f, Vec2(0, -10)), MoveBy::create(0.3f, Vec2(0, 10)), NULL));
                    _cursor->runAction(act);
                    _waitProgress = true;
                }
            });
            break;
        case ScriptFuncType::SetName:
            _nameText->setString(libspiral::any_cast<NameContext>(context.second.getContext()).name);
            _engine.progress();
            break;
        case ScriptFuncType::PlaceCharacter: {
            CharacterContext data = libspiral::any_cast<CharacterContext>(context.second.getContext());
            
            auto string = StringUtils::format("Characters/%02d/%02d_%02d.png", data.characterId, data.pictureId, data.faceId);
            auto sprite = Sprite::create(string);
            _bg->addChild(sprite);
            
            sprite->setPosition(_characterAnchors[data.position]->getPosition());
            sprite->setScale(0.7f);
            
            _engine.progress();
            break;
        }
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

