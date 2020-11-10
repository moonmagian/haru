#include "main_window.h"
#include "ui_main_window.h"
#include "game_entry.h"
#include <QtGlobal>
#include <QPair>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileIconProvider>
main_window::main_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::main_window), game(nullptr) {
    ui->setupUi(this);
    ui->gamelist->setLayout(ui->gamelist_layout);
    refresh_game_list();
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

void main_window::refresh_game_list() {
    auto games = game_entry::get_game_list();
    for (const auto &g : games) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(g.name);
        item->setData(Qt::UserRole, g.executable);
        item->setData(Qt::UserRole + 1, g.param);
        if (!g.icon.isEmpty()) {
            item->setIcon(QIcon(g.icon));
        } else {
            // extract icon from file.
            QFileInfo fin(g.executable);
            QFileSystemModel *model = new QFileSystemModel;
            model->setRootPath(fin.path());
            QFileIconProvider *ip = model->iconProvider();
            item->setIcon(ip->icon(fin));
            delete model;
        }
        ui->game_table->addItem(item);
    }
}
