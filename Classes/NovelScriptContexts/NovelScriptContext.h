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
    libspiral::Any& getContext() { return _context; }
    const libspiral::Any& getContext() const { return _context; }
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
    int position;
};

template<class ActionSet>
struct ActionSetContext {
    ActionSet* parent;
    ActionSet set;
};

struct TargetContext {
    int targetType;
    int id;
    float anchorX;
    float anchorY;
};

struct MoveContext {
    int x, y;
    float duration;
};

struct ScaleContext {
    float x, y;
    float duration;
};

struct RotateContext {
    int angle;
    float duration;
};

#endif /* NovelScriptContext_h */
