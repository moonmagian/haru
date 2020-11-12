#ifndef LUA_WORKER_HTTP_EXTENSION_H
#define LUA_WORKER_HTTP_EXTENSION_H
#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
namespace lua_worker_http_extension {
int http_get(lua_State *L);
int http_post(lua_State *L);
static constexpr struct luaL_Reg haru_ext[] = {
    {"http_get", http_get}, {"http_post", http_post}, {nullptr, nullptr}};
int load_lib(lua_State *L);
}; // namespace lua_worker_http_extension

#endif // LUA_WORKER_HTTP_EXTENSION_H
