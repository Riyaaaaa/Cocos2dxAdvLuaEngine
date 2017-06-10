//
//  NovelScriptEngine.cpp
//  Cocos2dxAdvLuaEngine
//
//  Created by Riya.Liel on 2017/06/10.
//
//

#include "NovelScriptEngine.h"

void NovelScriptEngine::progress() {
    _index++;
    for (auto && order :  _scriptOrders[_index]) {
        _handler(order);
    }
}