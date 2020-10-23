#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "translate_entry_widget.h"
#include "textractor_wrapper.h"
#include "hook_display_widget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class game_window;
}
QT_END_NAMESPACE

class game_window : public QMainWindow {
    Q_OBJECT

    public:
    game_window(QWidget *parent = nullptr);
    ~game_window();

    private slots:
    void on_debug_button_pushtext_clicked();
    void update_text(const QString &text);
    void got_hook_text(QString name, QString value);

    private:
    Ui::game_window *ui;
    textractor_wrapper textractor;
    QString using_hook;
    QMap<QString, hook_display_widget *> hook_widgets;
    int x;
    QVector<translate_entry_widget *> translating_entries;
    void init_plugins();
};
#endif // GAME_WINDOW_H
