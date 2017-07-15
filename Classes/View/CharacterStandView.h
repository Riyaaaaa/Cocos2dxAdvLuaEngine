//
//  CharacterStandView.hpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/07/15.
//
//

#ifndef CharacterStandView_hpp
#define CharacterStandView_hpp

#include "GradationSprite.h"
#include "../NovelScriptContexts/NovelScriptContext.h"

class CharacterStandView : public GradationSprite {
public:
    static CharacterStandView* create(std::string filename);
    bool init(std::string filename);
    void setContext(const CharacterContext& context) { _context = context; }
    CharacterContext getContext() {  return _context; }
private:
    CharacterContext _context;
};

#endif /* CharacterStandView_hpp */
