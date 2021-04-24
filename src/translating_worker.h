#ifndef TRANSLATING_WORKER_H
#define TRANSLATING_WORKER_H

#include <QThread>
#include <stdlib.h>
class translating_worker : public QThread {
    Q_OBJECT

    protected:
    QString path;
    QString text;
    unsigned long freshness;

    public:
    void set_text(const QString &text);
    void set_path(const QString &path);
    void run() override = 0;
    virtual void init() = 0;
    unsigned long get_freshness() const;
    void set_freshness(unsigned long value);

    signals:
    void translation_done(QString text);
};

#endif // TRANSLATING_WORKER_H
