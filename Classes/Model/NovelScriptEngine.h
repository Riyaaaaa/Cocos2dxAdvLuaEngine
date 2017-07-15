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
    
    void run(std::string filename);
    void progress();
    
    void setScriptHandler(std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> handler) { _handler = handler; }
public:
    typedef std::pair<ScriptFuncType, NovelScriptContext> action_t;
    typedef std::vector<action_t> action_set_t;
    
    cocos2d::LuaEngine* _engine;
    std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> _handler;
    
    void addAction(const action_t& action);
    
public:
    int index;
    action_set_t* _parentActionSet;
    action_set_t* _currentActionSet;
    action_set_t _routeActionSet;
};

#endif /* NovelScriptEngine_h */
