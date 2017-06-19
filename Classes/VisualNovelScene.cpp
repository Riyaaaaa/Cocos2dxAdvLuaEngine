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
#include "Utility/SpriteUtils.h"

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
            this->enableTextCursor(false);
        }
        
        if (_touchHandler) {
            _touchHandler();
        }
        
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

float VisualNovelScene::getDurationScriptFuncType(ScriptFuncType funcType) {
    switch (funcType) {
        case ScriptFuncType::PlaceCharacter:
            return 0.2f;
            break;
        default:
            break;
    }
    
    return 0.0f;
}

void VisualNovelScene::enableTextCursor(bool enable) {
    if (enable) {
        _cursor->setVisible(true);
        auto act = RepeatForever::create(Sequence::create(MoveBy::create(0.3f, Vec2(0, -10)),
                                                      MoveBy::create(0.3f, Vec2(0, 10)), nullptr));
        _cursor->runAction(act);
        _waitProgress = true;
    } else {
        _cursor->setVisible(false);
        _cursor->stopAllActions();
    }
}

void VisualNovelScene::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text: {
            auto text = libspiral::any_cast<NovelContext>(context.second.getContext()).text;
            _talkText->setString(text);
            NovelTextUtils::runCaption(_talkText, 0.1, [this](){
                if (_isAuto) {
                    _engine.progress();
                } else {
                    this->enableTextCursor(true);
                }
                _touchHandler = nullptr;
            });
            _touchHandler = [this, text]() {
                NovelTextUtils::clearTextAnimation(_talkText);
                _talkText->setString(text);
                this->enableTextCursor(true);
                _touchHandler = nullptr;
            };
            break;
        }
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
            
            SpriteUtils::fadeIn(sprite, getDurationScriptFuncType(context.first),
                                [this](){ _engine.progress(); });
            
            break;
        }
        case ScriptFuncType::End:
            break;
        case ScriptFuncType::Sleep:
            auto duration = libspiral::any_cast<float>(context.second.getContext());
            this->runAction(Sequence::create(DelayTime::create(duration),
                                             CallFunc::create([this](){ _engine.progress(); }));
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

