#include "game_window.h"
#include "ui_game_window.h"
#include "translate_entry_widget.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtGlobal>
#include <QToolButton>
#include <QTabBar>
#include <QCloseEvent>
#include <QDesktopWidget>
game_window::game_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::game_window), using_hook(), pause(false),
      drag_flag(false), resize_flag(false) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint |
                         Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint);
    //    this->setWindowFlags(Qt::NoDropShadowWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->translate_tab->setLayout(ui->translate_layout);
    ui->hook_tab->setLayout(ui->hook_layout);
    ui->history_tab->setLayout(ui->history_layout);
    //    ui->title->setAttribute(Qt::WA_TransparentForMouseEvents);
    //    ui->main_tab->setCornerWidget(ui->corner_toolbar);
    init_plugins();
    ui->hook_scroll_contents->setLayout(ui->hook_scroll_list);
    connect(&textractor, &textractor_wrapper::updated_hook_text, this,
            &game_window::got_hook_text);
    setMouseTracking(false);
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
    if (pause) {
        return;
    }
    ui->translate_text->setText(text);
    ui->history->moveCursor(QTextCursor::End);
    ui->history->insertPlainText(QString("%1\n").arg(text));
    ui->history->moveCursor(QTextCursor::End);
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

void game_window::got_new_translated_text(QString name, QString value) {
    // Append history.
    ui->history->moveCursor(QTextCursor::End);
    ui->history->insertPlainText(QString("[%1] %2\n").arg(name, value));
    ui->history->moveCursor(QTextCursor::End);
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
        connect(w, &translate_entry_widget::execution_finished, this,
                &game_window::got_new_translated_text);
        ui->translate_result_layout->addWidget(w);
    }
}

void game_window::closeEvent(QCloseEvent *cevent) {
    textractor.terminate();
    emit exit_game_window();
    cevent->accept();
}

void game_window::mousePressEvent(QMouseEvent *event) {
    auto mouse_pos = event->globalPos();
    auto mouse_pos_rel = ui->title->parentWidget()->mapFromGlobal(mouse_pos);
    if (ui->title->geometry().contains(mouse_pos_rel)) {
        mouse_move_begin_pos =
            ui->title->parentWidget()->mapToGlobal(mouse_pos_rel);
        window_move_begin_pos = pos();
        QGuiApplication::setOverrideCursor(Qt::SizeAllCursor);
        drag_flag = true;
    } else if (ui->drag_sign->geometry().contains(mouse_pos_rel)) {
        //        QCursor::setPos(this->geometry().bottomRight());
        QApplication::desktop()->cursor().setPos(
            this->geometry().bottomRight());
        mouse_move_begin_pos = this->geometry().bottomRight();
        window_original_size = this->size();
        QGuiApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        resize_flag = true;
    }
}

void game_window::mouseReleaseEvent(QMouseEvent *event) {
    if (drag_flag) {
        QGuiApplication::restoreOverrideCursor();
        drag_flag = false;
    } else if (resize_flag) {
        QGuiApplication::restoreOverrideCursor();
        resize_flag = false;
    }
}

void game_window::mouseMoveEvent(QMouseEvent *event) {
    if (drag_flag) {
        this->move(window_move_begin_pos + event->globalPos() -
                   mouse_move_begin_pos);
    } else if (resize_flag) {
        QSize nsize;
        auto delta = event->globalPos() - mouse_move_begin_pos;
        nsize.setWidth(window_original_size.width() + delta.x());
        nsize.setHeight(window_original_size.height() + delta.y());
        this->resize(nsize);
    }
}

void game_window::on_pause_tool_button_toggled(bool checked) {
    pause = checked;
}

void game_window::on_close_tool_button_clicked() { close(); }

void game_window::on_close_tool_button_toggled(bool checked) {}
