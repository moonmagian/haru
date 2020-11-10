#include "game_entry.h"
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
QVector<game_entry> game_entry::get_game_list() {
    QVector<game_entry> game_list;
    QFile config("games.json");
    if (!config.open(QFile::ReadOnly)) {
        qCritical("Can't open configuration file: games.json.");
        return game_list;
    }
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(config.readAll(), &err);
    config.close();
    if (err.error != QJsonParseError::NoError) {
        qCritical("Can't parse configuration file.");
        return game_list;
    }
    if (!document.isArray()) {
        qCritical("Incorrect format for games.json.");
        return game_list;
    }
    auto games = document.array();
    for (auto i : games) {
        if (!i.isObject()) {
            qCritical("Incorrect format for a game in games.json.");
            continue;
        }
        auto game = i.toObject();
        game_entry current_game_entry;
        if (!game.contains("name") || !game["name"].isString()) {
            qCritical("A name string is necessary for every game.");
            continue;
        }
        current_game_entry.name = game["name"].toString();
        if (!game.contains("executable") || !game["executable"].isString()) {
            qCritical("An executable is necessary for every game.");
            continue;
        }
        current_game_entry.executable = game["executable"].toString();
        if (game.contains("param") && game["param"].isString()) {
            current_game_entry.param = game["param"].toString();
        }
        if (game.contains("icon") && game["icon"].isString()) {
            current_game_entry.icon = game["icon"].toString();
        }
        game_list.append(std::move(current_game_entry));
    }
    return game_list;
}
