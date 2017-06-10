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
    libspiral::Any getContext() { return context; }
private:
    libspiral::Any context;
};

struct NovelContext {
    std::string text;
};

#endif /* NovelScriptContext_h */
