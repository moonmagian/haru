#include "game_window.h"
#include "ui_game_window.h"
#include "translate_entry_widget.h"
game_window::game_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::game_window) {
    ui->setupUi(this);
    // this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->translate_tab->setLayout(ui->translate_layout);

    w = new translate_entry_widget(this);
    w->set_script("moontrans.lua");
    w->set_name("test");
    ui->translate_layout->addWidget(w);
    x = 0;
}

game_window::~game_window() { delete ui; }

void game_window::on_debug_button_pushtext_clicked() {
    x += 1;
    ui->debug_button_pushtext->setText(QString::number(x));
    w->push_text(QString::number(x));
}
