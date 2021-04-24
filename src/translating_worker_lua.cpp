#include "translating_worker_lua.h"
//#include "lua_worker_http_extension.h"
#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
#include <QMutex>
#include <QDir>
translating_worker_lua::translating_worker_lua() : L(nullptr), inited(false) {}

void translating_worker_lua::init() {
    std::string script_stdstr = path.toStdString();
    if (L != nullptr) {
        lua_close(L);
        L = nullptr;
    }
    L = luaL_newstate();
    // lua_worker_http_extension::load_lib(L);
    luaL_openlibs(L);

    // Load 3rd part packages.
    lua_getglobal(L, "package");
    auto working_directory = QDir::currentPath();
    lua_pushstring(
        L,
        QString("%1/libs/lua_modules/share/lua/5.3/?.lua;%1/libs/lua_modules/"
                "share/lua/5.3/?/init.lua;%1/libs/lua_modules/lib/lua/5.3/"
                "?.lua;%1/libs/lua_modules/lib/lua/5.3/?/init.lua")
            .arg(working_directory)
            .toStdString()
            .c_str());
    lua_setfield(L, -2, "path");
#ifdef USE_WINE
    lua_pushstring(L, QString("%1/libs/lua_modules/lib/lua/5.3/?.so")
                          .arg(working_directory)
                          .toStdString()
                          .c_str());
#else
    lua_pushstring(L, QString("%1/libs/lua_modules/lib/lua/5.3/?.dll")
                          .arg(working_directory)
                          .toStdString()
                          .c_str());

#endif
    lua_setfield(L, -2, "cpath");
    lua_setglobal(L, "package");

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
        error = "[Error] The script file can't be read or contains "
                "syntax error.";
        qCritical("The script file can't be read or contains "
                  "syntax error, %s",
                  lua_tostring(L, -1));
        return;
    }
    inited = true;
}
void translating_worker_lua::run() {
    mutex.lock();
    if (!inited) {
        emit translation_done(error);
        mutex.unlock();
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
            mutex.unlock();
            return;
        }
    } else {
        emit translation_done("[Error] Can't call the translate function.");
        qCritical("Can't call the translate function, %s", lua_tostring(L, -1));
        mutex.unlock();
        return;
    }
    // Remove the function from the stack.
    lua_pop(L, lua_gettop(L));
    emit translation_done(result);
    mutex.unlock();
    return;
}

translating_worker_lua::~translating_worker_lua() {
    mutex.lock();
    if (L != nullptr) {
        lua_close(L);
    }
    mutex.unlock();
}
