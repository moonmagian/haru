#include "translate_entry_widget.h"
#include "ui_translate_entry_widget.h"
#include "translating_worker_lua.h"
#include <iostream>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <QGraphicsDropShadowEffect>
translate_entry_widget::translate_entry_widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::translate_entry_widget),
      worker(new translating_worker_lua) {
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout);
    //    connect(this, &translate_entry_widget::execution_finished, this,
    //            &translate_entry_widget::update_execution_result);
    worker->connect(worker, &translating_worker::translation_done, this,
                    &translate_entry_widget::update_execution_result);
    freshness = 0;
    QGraphicsDropShadowEffect *dse = new QGraphicsDropShadowEffect(this);
    dse->setBlurRadius(5);
    dse->setColor(QColor(255, 192, 203));
    dse->setXOffset(0);
    dse->setYOffset(0);
    ui->content->setGraphicsEffect(dse);
}

translate_entry_widget::~translate_entry_widget() {
    delete worker;
    delete ui;
}

void translate_entry_widget::set_name(const QString &name) {
    ui->name->setText(name);
}

void translate_entry_widget::set_script(const QString &script) {
    this->script = script;
    worker->set_path(script);
}

QString translate_entry_widget::get_name() { return ui->name->text(); }

void translate_entry_widget::push_text(const QString &text) {
    current_text = text;
    ++freshness;
    if (!worker->isRunning()) {
        worker->set_freshness(freshness);
        worker->set_text(current_text);
        worker->start();
    }
}

void translate_entry_widget::init_worker() {
    if (worker != nullptr) {
        worker->init();
    }
}

void translate_entry_widget::update_execution_result(const QString &result) {
    // Out-dated result, drop it.
    if (freshness != worker->get_freshness()) {
        if (!worker->isRunning()) {
            worker->set_freshness(freshness);
            worker->set_text(current_text);
            worker->start();
        }
    } else {
        ui->content->setText(result);
        emit execution_finished(ui->name->text(), result);
    }
}
