#ifndef GAME_ENTRY_H
#define GAME_ENTRY_H
#include <QString>

struct game_entry {
    QString name;
    QString executable;
    QString param;
    QString icon;
    static QVector<game_entry> get_game_list();
};

#endif // GAME_ENTRY_H
