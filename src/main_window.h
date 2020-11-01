#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "game_window.h"
namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
    Q_OBJECT

    public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();

    private slots:
    void on_pushButton_clicked();
    void gamewindow_close();

    void on_refresh_clicked();

    private:
    game_window *game;
    Ui::main_window *ui;
};

#endif // MAIN_WINDOW_H
