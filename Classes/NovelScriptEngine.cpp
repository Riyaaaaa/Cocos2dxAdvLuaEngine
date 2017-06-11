//
//  NovelScriptEngine.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "NovelScriptEngine.h"

USING_NS_CC;

static int setName(lua_State *L) {
    std::string name = lua_tostring(L, 1);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, 1));
    
    NameContext data;
    data.name = name;
    
    NovelScriptContext context;
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_scriptOrders[engine->_index].push_back(std::make_pair(ScriptFuncType::SetName, context));
    
    return 0;
}

NovelScriptEngine::NovelScriptEngine() {
    _scriptOrders.resize(1);
    _index = 0;
    
    _engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(_engine);
    auto* tolua_S = _engine->getLuaStack()->getLuaState();
    
    tolua_open(tolua_S);
    
    tolua_module(tolua_S, NULL, 0);
    tolua_beginmodule(tolua_S, NULL);
        tolua_function(tolua_S, "setName", &setName);
    tolua_endmodule(tolua_S);
    
    tolua_pushuserdata(tolua_S, this);
    lua_setglobal(tolua_S, "_instance");
    
    _engine->executeScriptFile("test.lua");
}

void NovelScriptEngine::progress() {
    _index++;
    for (auto && order :  _scriptOrders[_index]) {
        _handler(order);
    }
}