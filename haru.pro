QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += "-Wno-old-style-cast"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/hook_display_widget.cpp \
    src/main.cpp \
    src/game_window.cpp \
    src/main_window.cpp \
    src/textractor_wrapper.cpp \
    src/translate_entry_widget.cpp \
    src/translating_worker.cpp \
    src/translating_worker_lua.cpp

HEADERS += \
    src/game_window.h \
    src/hook_display_widget.h \
    src/main_window.h \
    src/textractor_wrapper.h \
    src/translate_entry_widget.h \
    src/translating_worker.h \
    src/translating_worker_lua.h

FORMS += \
    src/main_window.ui \
    src/ui/hook_display_widget.ui \
    src/ui/game_window.ui \
    src/ui/translate_entry_widget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#LIBS += -llua5.3
LIBS += -llua

unix: INCLUDEPATH += /usr/include/lua5.3
unix: DEPENDPATH += /usr/include/lua5.3
unix: DEFINES += USE_WINE

RESOURCES += \
    src/resources/images.qrc
