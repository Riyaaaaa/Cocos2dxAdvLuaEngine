//
//  NovelViewController.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/08.
//
//

#include "NovelViewController.h"

#include "../NovelScriptContexts/ScriptConst.h"
#include "../Utility/NovelTextUtils.h"
#include "../Utility/SpriteUtils.h"
#include "../Utility/ActionUtils.h"
#include "../View/CharacterStandView.h"

USING_NS_CC;

constexpr const char* CHARACTER_TAG_PREFIX = "Characeter";

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
    
    _scene->setEventCallBack([this](ViewEventType e){
        switch(e) {
            case ViewEventType::Enter:
                _engine.progress();
                break;
        }
    });
    
    _scene->retain();
    
    auto scene = Scene::create();
    scene->addChild(LayerColor::create(Color4B::BLACK));
    scene->setonEnterTransitionDidFinishCallback([this, filename](){
        _engine.run(filename);
        _engine.progress();
    });
    
    Director::getInstance()->runWithScene(scene);
}

NovelViewController::NovelViewController() {
    _engine.setScriptHandler(CC_CALLBACK_1(NovelViewController::scriptHandler, this));
    
    _isAuto = false;
    _waitProgress = false;
    _progressLock = false;
    target = nullptr;
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
            auto sprite = CharacterStandView::create(string);
            sprite->setName(std::string(CHARACTER_TAG_PREFIX) + std::to_string(data.characterId));
            _scene->getBg()->addChild(sprite);
            sprite->setContext(data);
            
            sprite->setPosition(_scene->getCharacterAnchor(data.position)->getPosition());
            sprite->setScale(0.7f);
            
            SpriteUtils::fadeIn(sprite, getDurationScriptFuncType(context.first),
                                [this](){ progress(); });
            
            break;
        }
        case ScriptFuncType::ReplaceFace: {
            CharacterContext data = libspiral::any_cast<CharacterContext>(context.second.getContext());
            
            auto sprite = _scene->getBg()->getChildByName(std::string(CHARACTER_TAG_PREFIX) + std::to_string(data.characterId));
            if (sprite) {
                auto standView = dynamic_cast<CharacterStandView*>(sprite);
                auto attachedContext = standView->getContext();
                auto filename = StringUtils::format("Characters/%02d/%02d_%02d.png",
                                                    attachedContext.characterId,
                                                    attachedContext.pictureId,
                                                    data.faceId);
                standView->setTexture(filename, [this](){
                    this->progress();
                });
            } else {
                CCLOG("Not found stand view. characterId = %d", data.characterId);
                progress();
            }
            
            break;
        }
        case ScriptFuncType::RemoveCharacter: {
            auto data = libspiral::any_cast<CharacterContext>(context.second.getContext());
            
            auto sprite = _scene->getBg()->getChildByName(std::string(CHARACTER_TAG_PREFIX) + std::to_string(data.characterId));
            if (sprite) {
                auto standView = dynamic_cast<CharacterStandView*>(sprite);
                standView->runAction(Sequence::create(FadeTo::create(0.5f, 0),
                                                      CallFunc::create([this](){ progress(); }),
                                                      RemoveSelf::create(),
                                                      nullptr));
            } else {
                CCLOG("Not found stand view. characterId = %d", data.characterId);
                progress();
            }

        }
            break;
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
            // auto data = libspiral::any_cast<std::pair<int, int>>(context.second.getContext());
            break;
        }
        case ScriptFuncType::Target: {
            auto data = libspiral::any_cast<TargetContext>(context.second.getContext());
            
            if (target) {
                target->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            }
            
            switch (data.targetType) {
            case ccnovel::TargetType::BG:
                target = _scene->getBg();
                break;
            case ccnovel::TargetType::Character:
                target = _scene->getBg()->getChildByName(std::string(CHARACTER_TAG_PREFIX) + std::to_string(data.id));
                break;
            }
            
            target->setAnchorPoint(Vec2(data.anchorX, data.anchorY));
            progress();
        }
            
        case ScriptFuncType::MoveTo: {
            auto data = libspiral::any_cast<MoveContext>(context.second.getContext());
            
            ActionUtils::runActionSeq(target, [this](){ progress(); }, MoveTo::create(data.duration, Vec2(data.x, data.y)));
        }
        case ScriptFuncType::MoveBy: {
            auto data = libspiral::any_cast<MoveContext>(context.second.getContext());
            
            ActionUtils::runActionSeq(target, [this](){ progress(); }, MoveBy::create(data.duration, Vec2(data.x, data.y)));
        }
        case ScriptFuncType::Run: {
            auto transitType = libspiral::any_cast<int>(context.second.getContext());
            
            switch (transitType) {
                default:
                    Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, _scene));
                    break;
            }
            _scene->release();
            break;
        }
        default:
            CCLOG("Unsupported instruction called.");
            progress();
            break;
    }
}

void NovelViewController::progress() {
    if (!_progressLock) {
        Director::getInstance()->getRunningScene()->runAction(CallFunc::create([this](){ _engine.progress(); }));
    }
}
