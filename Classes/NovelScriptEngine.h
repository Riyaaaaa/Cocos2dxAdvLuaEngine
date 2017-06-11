//
//  NovelScriptEngine.h
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#ifndef NovelScriptEngine_h
#define NovelScriptEngine_h

#include <functional>

#include "NovelScriptContexts/NovelScriptContext.h"
#include "NovelScriptContexts/ScriptFuncType.h"

#include "CCLuaEngine.h"
#include "lua_module_register.h"

class NovelScriptEngine {
public:
    NovelScriptEngine();
    void progress();
    
    void setScriptHandler(std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> handler);
public:
    cocos2d::LuaEngine* _engine;
    std::size_t _index;
    std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> _handler;
    
public:
    std::vector<std::vector<std::pair<ScriptFuncType, NovelScriptContext>>> _scriptOrders;
};

#endif /* NovelScriptEngine_h */
