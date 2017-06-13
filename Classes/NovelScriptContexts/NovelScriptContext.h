//
//  NovelScriptContext.h
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#ifndef NovelScriptContext_h
#define NovelScriptContext_h

#include "Any.hpp"

class NovelScriptContext {
public:
    ~NovelScriptContext() = default;
    libspiral::Any getContext() { return _context; }
    void setContext(const libspiral::Any& context) { _context = context; }
private:
    libspiral::Any _context;
};

struct NovelContext {
    std::string text;
};

struct NameContext {
    std::string name;
};

struct CharacterContext {
    int characterId;
    int pictureId;
    int faceId;
};

#endif /* NovelScriptContext_h */
