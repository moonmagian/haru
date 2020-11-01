#include "game_window.h"
#include "main_window.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <iostream>
#include <QFontDatabase>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qSetMessagePattern(
        "[%{time yyyyMMdd h:mm:ss.zzz t} "
        "%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-"
        "critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    auto style = QStyleFactory::create("Fusion");
    qApp->setStyle(style);
    //    qApp->setStyleSheet("QToolButton { background-color: red; border:
    //    none; }");

    //    game_window w;
    main_window w;

    w.show();
    return a.exec();
}
