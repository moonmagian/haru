#include "game_window.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qSetMessagePattern(
        "[%{time yyyyMMdd h:mm:ss.zzz t} "
        "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-"
        "critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    game_window w;
    w.show();
    return a.exec();
}
