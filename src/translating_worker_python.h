#ifndef TRANSLATING_WORKER_PYTHON_H
#define TRANSLATING_WORKER_PYTHON_H

#include "translating_worker.h"
#include <lua.hpp>
#include <QMutex>
#include <QProcess>
class translating_worker_python : public translating_worker {
    private:
    QMutex mutex;

    public:
    translating_worker_python();
    void init() override;
    void run() override;
    ~translating_worker_python() override;
};

#endif // TRANSLATING_WORKER_PYTHON_H
