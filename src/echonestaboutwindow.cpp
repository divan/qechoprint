#include "src/echonestaboutwindow.h"
#include "ui_echonestaboutwindow.h"

#include <QDesktopServices>
#include <QtGui>

EchoNestAboutWindow::EchoNestAboutWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EchoNestAboutWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
    setAttribute(Qt::WA_Maemo5AutoOrientation);
#endif

    connect(QApplication::desktop(), SIGNAL(resized(int)),
            this, SLOT(orientationChanged()));
    ui->setupUi(this);
    orientationChanged();

    QString text;
    text += tr("Echoprint is a fully open-source, end-to-end music fingerprinter "
               "with its own large reference catalog. "
               "It is powered by The Echo Nest and launching in partnership with Musicbrainz"
               " and 7digital.");
    ui->textLabel->setText(text);
}

EchoNestAboutWindow::~EchoNestAboutWindow()
{
    delete ui;
}

void EchoNestAboutWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(this->rect(),
                      QImage(":/images/bg.jpg"));
}

void EchoNestAboutWindow::orientationChanged()
{
#ifdef Q_WS_MAEMO_5
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    // Landscape
    if (screenGeometry.width() > screenGeometry.height())
    {
    }
    else // Portrait
    {
    }
#endif
}

void EchoNestAboutWindow::on_urlButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://the.echonest.com"));
}
