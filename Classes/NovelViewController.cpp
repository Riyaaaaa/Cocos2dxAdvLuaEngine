//
//  NovelViewController.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/08.
//
//

#include "NovelViewController.h"

#include "Utility/NovelTextUtils.h"
#include "Utility/SpriteUtils.h"

USING_NS_CC;

void NovelViewController::runScene(std::string filename, int transitType)
{
    _scene = VisualNovelScene::createScene();
    
    _scene->setTouchCallback([this](Touch*, Event*){
        if (_waitProgress) {
            _engine.progress();
            _waitProgress = false;
            this->enableTextCursor(false);
        }
        
        if (_touchHandler) {
            _touchHandler();
        }
        return true;
    });
    
    _scene->setEventCallBack([this, filename](ViewEventType e){
        switch(e) {
            case ViewEventType::Enter:
                _engine.run(filename);
                _engine.progress();
                break;
        }
    });
    
    Director::getInstance()->runWithScene(_scene);
}

NovelViewController::NovelViewController() {
    _engine.setScriptHandler(CC_CALLBACK_1(NovelViewController::scriptHandler, this));
    
    _isAuto = false;
    _waitProgress = false;
    _progressLock = false;
    
    Director::getInstance()->runWithScene(Scene::create());
}

float NovelViewController::getDurationScriptFuncType(ScriptFuncType funcType) {
    switch (funcType) {
        case ScriptFuncType::PlaceCharacter:
            return 0.2f;
            break;
        default:
            break;
    }
    
    return 0.0f;
}

void NovelViewController::enableTextCursor(bool enable) {
    if (enable) {
        _scene->getCursor()->setVisible(true);
        auto act = RepeatForever::create(Sequence::create(MoveBy::create(0.3f, Vec2(0, -10)),
                                                          MoveBy::create(0.3f, Vec2(0, 10)), nullptr));
        _scene->getCursor()->runAction(act);
        _waitProgress = true;
    } else {
        _scene->getCursor()->setVisible(false);
        _scene->getCursor()->stopAllActions();
    }
}

void NovelViewController::scriptHandler(std::pair<ScriptFuncType, NovelScriptContext> context) {
    switch (context.first) {
        case ScriptFuncType::Text: {
            auto text = libspiral::any_cast<NovelContext>(context.second.getContext()).text;
            _scene->getTalkText()->setString(text);
            NovelTextUtils::runCaption(_scene->getTalkText(), 0.1, [this](){
                if (_isAuto) {
                    progress();
                } else {
                    this->enableTextCursor(true);
                }
                _touchHandler = nullptr;
            });
            _touchHandler = [this, text]() {
                NovelTextUtils::clearTextAnimation(_scene->getTalkText());
                _scene->getTalkText()->setString(text);
                this->enableTextCursor(true);
                _touchHandler = nullptr;
            };
            break;
        }
        case ScriptFuncType::SetName:
            _scene->getNameText()->setString(libspiral::any_cast<NameContext>(context.second.getContext()).name);
            progress();
            break;
        case ScriptFuncType::PlaceCharacter: {
            CharacterContext data = libspiral::any_cast<CharacterContext>(context.second.getContext());
            
            auto string = StringUtils::format("Characters/%02d/%02d_%02d.png", data.characterId, data.pictureId, data.faceId);
            auto sprite = Sprite::create(string);
            _scene->getBg()->addChild(sprite);
            
            sprite->setPosition(_scene->getCharacterAnchor(data.position)->getPosition());
            sprite->setScale(0.7f);
            
            SpriteUtils::fadeIn(sprite, getDurationScriptFuncType(context.first),
                                [this](){ progress(); });
            
            break;
        }
        case ScriptFuncType::End:
            break;
        case ScriptFuncType::Sleep: {
            auto duration = libspiral::any_cast<float>(context.second.getContext());
            _scene->runAction(Sequence::create(DelayTime::create(duration),
                                             CallFunc::create([this](){ progress(); }),
                                             nullptr));
            break;
        }
        case ScriptFuncType::SetBg: {
            int bgId = libspiral::any_cast<int>(context.second.getContext());
            _scene->getBg()->setTexture(StringUtils::format("Bg/%02d.png", bgId));
            progress();
            break;
        }
        case ScriptFuncType::Spawn: {
            auto action_set = libspiral::any_cast<ActionSetContext<NovelScriptEngine::action_set_t>>(context.second.getContext());
            
            auto it = std::max_element(action_set.set.begin(),
                                       action_set.set.end(),
                                       [this](const NovelScriptEngine::action_t& lhs,
                                              const NovelScriptEngine::action_t& rhs){
                                           return getDurationScriptFuncType(lhs.first) < getDurationScriptFuncType(rhs.first);
                                       });
            
            _progressLock = true;
            
            _scene->runAction(Sequence::create(DelayTime::create(getDurationScriptFuncType(it->first)),
                                             CallFunc::create([this](){
                _progressLock = false;
                progress();
            }),
                                             nullptr));
            
            // TODO: Do not run progress on child action
            for (auto && action : action_set.set) {
                scriptHandler(action);
            }
            break;
        }
        case ScriptFuncType::Transit: {
            auto data = libspiral::any_cast<std::pair<int, int>>(context.second.getContext());
            break;
        }
        default:
            progress();
            break;
    }
}

void NovelViewController::progress() {
    if (!_progressLock) {
        _engine.progress();
    }
}
