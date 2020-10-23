#ifndef HOOK_DISPLAY_WIDGET_H
#define HOOK_DISPLAY_WIDGET_H

#include <QWidget>

namespace Ui {
class hook_display_widget;
}

class hook_display_widget : public QWidget {
    Q_OBJECT

    public:
    explicit hook_display_widget(QWidget *parent = nullptr);
    ~hook_display_widget();
    void set_name(const QString &name);
    void set_value(const QString &value);
    QString get_value();
    signals:
    void use_hook(QString name);
    private slots:
    void on_use_button_clicked();

    private:
    Ui::hook_display_widget *ui;
};

#endif // HOOK_DISPLAY_WIDGET_H
