#ifndef TRANSLATING_WORKER_PYTHON_H
#define TRANSLATING_WORKER_PYTHON_H

#include "translating_worker.h"
#include <lua.hpp>
#include <QMutex>
class translating_worker_lua : public translating_worker {
    private:
    lua_State *L;

    public:
    translating_worker_lua();
    void init() override;
    void run() override;
    ~translating_worker_lua() override;
    bool inited;
    QString error;
};

#endif // TRANSLATING_WORKER_PYTHON_H
