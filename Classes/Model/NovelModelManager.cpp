//
//  NovelModelManager.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/08.
//
//

#include "NovelModelManager.h"

void NovelModelManager::runScene() {
    _controller = std::make_shared<NovelViewController>();
    _controller->runScene("ukano.lua");
}