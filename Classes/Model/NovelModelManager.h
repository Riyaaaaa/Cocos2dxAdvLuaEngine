//
//  NovelModelManager.hpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/08.
//
//

#ifndef NovelModelManager_h
#define NovelModelManager_h

#include "../SpiralLibrary/GameSystem/SingletonBase.hpp"
#include "../Controller/NovelViewController.h"

class NovelModelManager : public libspiral::SingletonBase<NovelModelManager> {
public:
    NovelModelManager() = default;
    
    void runScene();
private:
    std::shared_ptr<NovelViewController> _controller;
};

#endif /* NovelModelManager_hpp */
