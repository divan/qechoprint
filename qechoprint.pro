# vim: ft=qmake

QT += network core gui
maemo5:QT += maemo5
debug:DEFINES += DEBUG

CONFIG += mobility
MOBILITY += multimedia network

# Uncomment it if want to use libechonest
#DEFINES += USE_LIBECHONEST

SOURCES += src/main.cpp src/mainwindow.cpp \
    src/resultwindow.cpp \
    src/codegenwrapper.cpp \
    src/recordermobility.cpp
HEADERS += src/mainwindow.h \
    src/resultwindow.h \
    src/codegenwrapper.h \
    src/recordermobility.h
FORMS += src/ui/mainwindow.ui \
    src/ui/resultwindow.ui

LIBS += -lcodegen

CONFIG += link_pkgconfig
PKGCONFIG += QJson

contains(DEFINES, USE_LIBECHONEST) {
    LIBS += -L$$PWD/codegen/libs -lechonest
    INCLUDEPATH += $$PWD/echonest/include
} else {
    SOURCES += src/echonestrequest.cpp \
            src/echonestsong.cpp
    HEADERS += src/echonestrequest.h \
            src/echonestsong.h
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    debian/rules \
    debian/README \
    debian/files \
    debian/copyright \
    debian/control \
    debian/compat \
    debian/changelog

RESOURCES += \
    src/qechoprint.qrc

# Add files and directories to ship with the application
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE5345F53

# Smart Installer packages UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
