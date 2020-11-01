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
    ~game_window() override;
    signals:
    void exit_game_window();
    private slots:
    void on_debug_button_pushtext_clicked();
    void update_text(const QString &text);
    void got_hook_text(QString name, QString value);
    void got_new_translated_text(QString name, QString value);

    void on_pause_tool_button_toggled(bool checked);

    void on_close_tool_button_clicked();

    void on_close_tool_button_toggled(bool checked);

    private:
    Ui::game_window *ui;
    textractor_wrapper textractor;
    QString using_hook;
    QMap<QString, hook_display_widget *> hook_widgets;
    bool pause;
    int x;

    QPoint mouse_move_begin_pos;
    QPoint window_move_begin_pos;
    bool drag_flag;
    QSize window_original_size;
    bool resize_flag;

    QVector<translate_entry_widget *> translating_entries;
    void init_plugins();

    protected:
    void closeEvent(QCloseEvent *cevent) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
#endif // GAME_WINDOW_H
