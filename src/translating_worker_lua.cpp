#include "translating_worker_lua.h"
#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
#include <QMutex>
translating_worker_lua::translating_worker_lua() : L(nullptr), inited(false) {}

void translating_worker_lua::init() {
    std::string script_stdstr = path.toStdString();
    if (L != nullptr) {
        lua_close(L);
        L = nullptr;
    }
    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, script_stdstr.c_str()) == LUA_OK) {
        if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
            lua_pop(L, lua_gettop(L));
        } else {
            error = "[Error] The script file can't be executed.";
            qCritical("The script file can't be executed, %s",
                      lua_tostring(L, -1));
            return;
        }
    } else {
        error =
            "[Error] The script file can't be read or contains syntax error.";
        qCritical("The script file can't be read or contains syntax error, %s",
                  lua_tostring(L, -1));
        return;
    }
    inited = true;
}
void translating_worker_lua::run() {
    if (!inited) {
        emit translation_done(error);
        return;
    }
    QString result;
    std::string text_stdstr = text.toStdString(); // Put function on stack.
    lua_getglobal(L, "translate");
    // Put the argument.
    lua_pushstring(L, text_stdstr.c_str());
    if (lua_pcall(L, 1, 1, 0) == LUA_OK) {
        // return the text.
        if (lua_isstring(L, -1)) {
            result = lua_tostring(L, -1);
            lua_pop(L, 1);
        } else {
            emit translation_done("[Error] Can't read the returned text.");
            qCritical("Can't read the returned text, %s", lua_tostring(L, -1));
            return;
        }
    } else {
        emit translation_done("[Error] Can't call the translate function.");
        qCritical("Can't call the translate function, %s", lua_tostring(L, -1));
        return;
    }
    // Remove the function from the stack.
    lua_pop(L, lua_gettop(L));
    emit translation_done(result);
    return;
}

translating_worker_lua::~translating_worker_lua() {
    if (L != nullptr) {
        lua_close(L);
    }
}
