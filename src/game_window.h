#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include "translate_entry_widget.h"
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

    private:
    Ui::game_window *ui;
    int x;
    translate_entry_widget *w;
};
#endif // GAME_WINDOW_H
