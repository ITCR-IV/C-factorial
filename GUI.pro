QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CodeEditor.cpp \
    Mainwindow.cpp \
    server/mserver.cpp \
    interpreter/Lexer.cpp \
    interpreter/Parser.cpp \
    interpreter/Token.cpp \
    main.cpp \

HEADERS += \
    CodeEditor.h \
    LineNumberArea.h \
    Mainwindow.h \
    server/mserver.h \
    interpreter/Lexer.h \
    interpreter/Parser.h \
    interpreter/Token.h \

FORMS += \
    Mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

DISTFILES += \
    interpreter/grammar.ebnf
