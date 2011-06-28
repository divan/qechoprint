#include "mainwindow.h"

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString localeName = QLocale::system().name();

    QTranslator translator;
    translator.load("qechoprint_" + localeName,
                    "/opt/qechoprint/translations/");
    app.installTranslator(&translator);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + localeName,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
    mainWindow.showExpanded();

    return app.exec();
}
