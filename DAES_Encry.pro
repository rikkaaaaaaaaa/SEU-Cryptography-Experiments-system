QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BigInt.cpp \
    DH.cpp \
    EIGamal.cpp \
    encryption.cpp \
    euclid.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    rsa.cpp

HEADERS += \
    BigInt.h \
    DH.h \
    EIGamal.h \
    encryption.h \
    euclid.h \
    loginwindow.h \
    mainwindow.h \
    rsa.h

FORMS += \
    DH.ui \
    EIGamal.ui \
    euclid.ui \
    loginwindow.ui \
    mainwindow.ui \
    rsa.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# win32 {
# INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
# LIBS += -L"C:/Program Files/OpenSSL-Win64/lib" -llibcrypto
# }

#OpenSSL
INCLUDEPATH += $$PWD/lib/OpenSSL/include
LIBS += -L$$PWD/lib/OpenSSL/ -llibcrypto-3-x64 -llibssl-3-x64

RESOURCES += \
    loginpic.qrc
