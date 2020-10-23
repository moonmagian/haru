#include "hook_display_widget.h"
#include "ui_hook_display_widget.h"

hook_display_widget::hook_display_widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::hook_display_widget) {
    ui->setupUi(this);
}

hook_display_widget::~hook_display_widget() { delete ui; }

void hook_display_widget::set_name(const QString &name) {
    ui->name->setText(name);
}

void hook_display_widget::set_value(const QString &value) {
    ui->value->setText(value);
}

QString hook_display_widget::get_value() { return ui->value->text(); }

void hook_display_widget::on_use_button_clicked() {
    emit use_hook(ui->name->text());
}
