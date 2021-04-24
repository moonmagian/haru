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
        game = new game_window(32, nullptr);
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
    auto plist = get_process_list();
    for (const auto &p : plist) {
        ui->process_list->addItem(QString("%1, %2").arg(p.name).arg(p.pid),
                                  p.pid);
    }
}

QVector<process_entry> main_window::get_process_list() {
    QVector<process_entry> result;
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
        result.append({single_process.last(), process_pid});
    }
#endif
#ifndef USE_WINE
    // Get task list using native windows api.
    QProcess p(this);
    QStringList arguments;
    arguments << "/fo csv /nh";
    p.start("tasklist", arguments);
    p.waitForFinished();
    QString list = QString::fromUtf8(p.readAllStandardOutput());
    qDebug("%s", list.toStdString().c_str());
    auto process_list = list.split("\n");
    for (const auto &x : process_list) {
        if (x.count() == 0)
            continue;
        auto single_process = x.split(",");
        QString pname = single_process[0].chopped(1).mid(1);
        int process_pid = single_process[1].chopped(1).mid(1).toInt();
        result.append({pname, process_pid});
    }
#endif
    return result;
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

void main_window::on_run_clicked() {
    if (ui->process_list->currentIndex() != -1) {
        if (!game) {
            game = new game_window(ui->process_list->currentData().toUInt(),
                                   nullptr);
            connect(game, &game_window::exit_game_window, this,
                    &main_window::gamewindow_close);
            game->show();
            this->hide();
        }
    }
}
