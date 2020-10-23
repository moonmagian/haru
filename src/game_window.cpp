#include "game_window.h"
#include "ui_game_window.h"
#include "translate_entry_widget.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtGlobal>
#include <QToolButton>
#include <QTabBar>
game_window::game_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::game_window), using_hook() {
    ui->setupUi(this);
    // this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->translate_tab->setLayout(ui->translate_layout);
    ui->hook_tab->setLayout(ui->hook_layout);
    //    w = new translate_entry_widget(this);
    //    w->set_script("moontrans.lua");
    //    w->set_name("test");
    //    ui->translate_layout->addWidget(w);
    init_plugins();
    ui->hook_scroll_contents->setLayout(ui->hook_scroll_list);
    connect(&textractor, &textractor_wrapper::updated_hook_text, this,
            &game_window::got_hook_text);
    textractor.attach(924);
    x = 0;
}

game_window::~game_window() {
    delete ui;

    for (auto x : translating_entries) {
        delete x;
    }
    for (auto x : hook_widgets) {
        delete x;
    }
}

void game_window::on_debug_button_pushtext_clicked() {
    x += 1;
    ui->debug_button_pushtext->setText(QString::number(x));
    update_text(QString::number(x));
}

void game_window::update_text(const QString &text) {
    ui->translate_text->setText(text);
    for (auto x : translating_entries) {
        x->push_text(text);
    }
}

void game_window::got_hook_text(QString name, QString value) {
    if (name == this->using_hook) {
        update_text(value);
    }
    // Update hook list.
    if (hook_widgets.contains(name)) {
        hook_widgets[name]->set_value(value);
    } else {
        hook_display_widget *widget = new hook_display_widget(this);
        hook_widgets.insert(name, widget);
        widget->set_name(name);
        widget->set_value(value);
        connect(widget, &hook_display_widget::use_hook, [this](QString name) {
            // Push current text.
            if (this->using_hook != name) {
                update_text(this->hook_widgets[name]->get_value());
            }
            this->using_hook = name;
        });
        ui->hook_scroll_list->addWidget(widget);
    }
}

void game_window::init_plugins() {
    QFile config("config.json");
    if (!config.open(QFile::ReadOnly)) {
        qCritical("Can't open configuration file: config.json.");
        return;
    }
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(config.readAll(), &err);
    config.close();
    if (err.error != QJsonParseError::NoError) {
        qCritical("Can't parse configuration file.");
        return;
    }
    auto plugins_json = document["plugins"];
    if (!plugins_json.isArray()) {
        qCritical("Can't find plugins in configuration.");
        return;
    }
    QJsonArray plugins = plugins_json.toArray();
    for (auto plugin : plugins) {
        auto plugin_str = plugin.toString();
        auto plugin_str_std = plugin_str.toStdString();
        QString plugin_dir = QString("plugins/%1/").arg(plugin_str);
        QFile plugin_meta(plugin_dir + "plugin.json");
        if (!plugin_meta.open(QFile::ReadOnly)) {
            qCritical("Can't find meta file of plugin: %s.",
                      plugin_str_std.c_str());
            continue;
        }
        QJsonDocument meta_document =
            QJsonDocument::fromJson(plugin_meta.readAll(), &err);
        plugin_meta.close();
        if (err.error != QJsonParseError::NoError) {
            qCritical("Can't parse meta file of plugin: %s.",
                      plugin_str_std.c_str());
            continue;
        }
        QJsonValue plugin_name = meta_document["name"];
        QJsonValue plugin_script = meta_document["script"];
        if (!plugin_name.isString()) {
            qCritical("Can't get name of plugin: %s.", plugin_str_std.c_str());
            continue;
        }
        if (!plugin_script.isString()) {
            qCritical("Can't get script of plugin: %s.",
                      plugin_str_std.c_str());
            continue;
        }
        translate_entry_widget *w = new translate_entry_widget(this);
        w->set_name(plugin_name.toString());
        w->set_script(
            QString("plugins/%1/%2").arg(plugin_str, plugin_script.toString()));
        translating_entries.append(w);
        ui->translate_result_layout->addWidget(w);
    }
}
