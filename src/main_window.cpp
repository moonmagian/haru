#include "main_window.h"
#include "ui_main_window.h"
#include <QtGlobal>
#include <QPair>
main_window::main_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::main_window), game(nullptr) {
    ui->setupUi(this);
    ui->gamelist->setLayout(ui->gamelist_layout);
}

main_window::~main_window() { delete ui; }

void main_window::on_pushButton_clicked() {
    if (!game) {
        game = new game_window(nullptr);
        connect(game, &game_window::exit_game_window, this,
                &main_window::gamewindow_close);
        game->show();
        this->hide();
    }
}

void main_window::gamewindow_close() {
    this->show();
    delete game;
    game = nullptr;
}

void main_window::on_refresh_clicked() {
    ui->process_list->clear();
#ifdef USE_WINE
    // Get task list using wine.
    QProcess p(this);
    QStringList arguments;
    arguments << "tasklist";
    p.start("wine", arguments);
    p.waitForFinished();
    QString list = QString::fromUtf8(p.readAllStandardOutput());
    qDebug("%s", list.toStdString().c_str());
    auto process_list = list.split("\n");
    for (const auto &x : process_list) {
        if (x.count() == 0)
            continue;
        auto single_process = x.split(",");
        int process_pid = single_process.last().toInt();
        single_process.removeLast();
        ui->process_list->addItem(x, process_pid);
    }
#endif
}
