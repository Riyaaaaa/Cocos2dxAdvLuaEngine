//
//  GradationSprite.hpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/15.
//
//

#ifndef GradationSprite_h
#define GradationSprite_h

#include "cocos2d.h"

class GradationSprite : public cocos2d::Sprite {
public:
    static GradationSprite* create(std::string filename);
    bool init(std::string filename);
    
    void setTexture(std::string filename, std::function<void()> callback = nullptr);
    
private:
    cocos2d::Sprite* _previousSprite;
};

#endif /* GradationSprite_hpp */
