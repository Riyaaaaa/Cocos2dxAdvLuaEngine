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

class NovelScriptEngine {
public:
    void progress();
    
    void setScriptHandler(std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> handler);
private:
    std::size_t _index;
    std::function<void(std::pair<ScriptFuncType, NovelScriptContext>)> _handler;
    std::vector<std::vector<std::pair<ScriptFuncType, NovelScriptContext>>> _scriptOrders;
};

#endif /* NovelScriptEngine_h */
