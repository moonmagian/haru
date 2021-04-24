#ifndef TRANSLATE_ENTRY_WIDGET_H
#define TRANSLATE_ENTRY_WIDGET_H

#include <QWidget>
#include "translating_worker.h"
enum class execution_codes { finished, error };

namespace Ui {
class translate_entry_widget;
}

class translate_entry_widget : public QWidget {
    Q_OBJECT
    signals:
    void execution_finished(QString name, QString result);

    public:
    translate_entry_widget(const QString &type, QWidget *parent = nullptr);
    ~translate_entry_widget() override;

    void set_name(const QString &name);
    void set_script(const QString &script);
    QString get_name();
    void push_text(const QString &text);
    void init_worker();
    public slots:
    void update_execution_result(const QString &result);

    private:
    Ui::translate_entry_widget *ui;
    QString current_text;
    unsigned long freshness;
    translating_worker *worker;
    QString script;
};

#endif // TRANSLATE_ENTRY_WIDGET_H
