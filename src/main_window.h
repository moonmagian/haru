#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "game_entry.h"
#include "game_window.h"
struct process_entry {
    QString name;
    int pid;
};

namespace Ui {
class main_window;
}

class main_window : public QMainWindow {
    Q_OBJECT

    public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window() override;

    private slots:
    void on_pushButton_clicked();
    void gamewindow_close();

    void on_refresh_clicked();
    QVector<process_entry> get_process_list();

    void on_run_clicked();

    private:
    Ui::main_window *ui;
    game_window *game;
    void refresh_game_list();
};

#endif // MAIN_WINDOW_H
