TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lueye_api
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lueye_api
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lueye_api

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include
