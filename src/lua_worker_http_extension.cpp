#include "lua_worker_http_extension.h"
#include <QString>
int lua_worker_http_extension::load_lib(lua_State *L) {
    if (L == nullptr) {
        return -1;
    }
    luaL_newlib(L, haru_ext);
    lua_setglobal(L, "haru_ext");
    return 0;
}

int lua_worker_http_extension::http_get(lua_State *L) {
    const char *addr_cc = luaL_checkstring(L, 1);
    QString addr(addr_cc);

    lua_pushstring(L, "get_test");
    return 1;
}

int lua_worker_http_extension::http_post(lua_State *L) {
    lua_pushstring(L, "post_test");
    return 1;
}
