//
//  NovelScriptEngine.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "NovelScriptEngine.h"

#include "LuaUtils.h"

USING_NS_CC;

static int setName(lua_State *L) {
    std::string name = lua_tostring(L, 1);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    NameContext data;
    data.name = name;
    
    NovelScriptContext context;
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::SetName, context));
    
    return 0;
}

static int setText(lua_State *L) {
    std::string text = lua_tostring(L, 1);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    NovelContext data;
    data.text = text;
    
    NovelScriptContext context;
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::Text, context));
    
    engine->_currentActionSet = engine->_parentActionSet;
    engine->_parentActionSet = libspiral::any_cast<ActionSetContext<NovelScriptEngine::action_set_t>>(engine->_parentActionSet).parent;
    
    return 0;
}

static int placeCharacter(lua_State *L) {
    int charaId = lua_tonumber(L, 1);
    int pictureId = lua_tonumber(L, 2);
    int faceId = lua_tonumber(L, 3);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    CharacterContext data;
    data.characterId = charaId;
    data.pictureId = pictureId;
    data.faceId = faceId;
    
    NovelScriptContext context;
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::PlaceCharacter, context));
    
    return 0;
}

static int replaceFace(lua_State *L) {
    int charaId = lua_tonumber(L, 1);
    int faceId = lua_tonumber(L, 2);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    CharacterContext data;
    data.characterId = charaId;
    data.pictureId = -1;
    data.faceId = faceId;
    
    NovelScriptContext context;
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::ReplaceFace, context));
    
    return 0;
}

static int sleep(lua_State *L) {
    float mtime = lua_tonumber(L, 1);
    
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    NovelScriptContext context;
    libspiral::Any any = mtime;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::Sleep, context));
    
    return 0;
}

static int spawnBegan(lua_State *L) {
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    NovelScriptContext context;
    
    ActionSetContext<NovelScriptEngine::action_set_t> data;
    data.parent = engine->_currentActionSet;
    data.set = NovelScriptEngine::action_set_t{};
    
    libspiral::Any any = data;
    context.setContext(any);
    
    engine->_currentActionSet->push_back(std::make_pair(ScriptFuncType::Spawn, context));
    engine->_parentActionSet = engine->_currentActionSet;
    engine->_currentActionSet = &libspiral::any_cast<ActionSetContext<NovelScriptEngine::action_set_t>>(&engine->_currentActionSet->back().second.getContext())->set;
    
    
    return 0;
}

static int spawnEnd(lua_State *L) {
    lua_getglobal(L, "_instance");
    NovelScriptEngine *engine = reinterpret_cast<NovelScriptEngine*>(lua_touserdata(L, lua_gettop(L)));
    
    engine->_currentActionSet = libspiral::any_cast<ActionSetContext<NovelScriptEngine::action_set_t>>(engine->_currentActionSet).parent;
    return 0;
}

NovelScriptEngine::NovelScriptEngine() {
    NovelScriptContext context;
    
    ActionSetContext<NovelScriptEngine::action_set_t> data;
    data.parent = nullptr;
    data.set = NovelScriptEngine::action_set_t{};
    
    libspiral::Any any = data;
    context.setContext(any);
    
    _routeActionSet.push_back(std::make_pair(ScriptFuncType::Spawn, context));
    
    _currentActionSet =
    _parentActionSet = nullptr; // route
    
    _engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(_engine);
    auto* tolua_S = _engine->getLuaStack()->getLuaState();
    
    tolua_open(tolua_S);
    
    tolua_module(tolua_S, NULL, 0);
    tolua_beginmodule(tolua_S, NULL);
    /* main features */
    tolua_function(tolua_S, "_N", &setName);
    tolua_function(tolua_S, "_T", &setText);
    tolua_function(tolua_S, "_C", &placeCharacter);
    tolua_function(tolua_S, "_R", &replaceFace);
    tolua_function(tolua_S, "_S", &sleep);
    tolua_function(tolua_S, "_S", &spawnBegan);
    tolua_function(tolua_S, "_S", &spawnEnd);
    
    tolua_endmodule(tolua_S);
    
    tolua_pushuserdata(tolua_S, this);
    lua_setglobal(tolua_S, "_instance");
    
    _engine->executeScriptFile("test.lua");
}

void NovelScriptEngine::progress() {
    for (auto && order :  _routeActionSet) {
        _handler(order);
    }
}