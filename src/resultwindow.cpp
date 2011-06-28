#include "resultwindow.h"
#include "ui_resultwindow.h"

#include <QtGui>

ResultWindow::ResultWindow(QWidget *parent, const EchoNestSong *song) :
    QMainWindow(parent),
    ui(new Ui::ResultWindow)
    ,_song(song)
{
    setAttribute(Qt::WA_DeleteOnClose);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    ui->setupUi(this);

    if (song == NULL)
    {
        ui->pixmapLabel->setPixmap(QPixmap(":/images/sad.png"));
        ui->statusLabel->setText(tr("Unable to identify the song"));
        ui->songButton->hide();
    }
    else
    {
        QString songDesc = QString("%1 - %2")
                .arg(song->artist())
                .arg(song->title());

        ui->statusLabel->setText(tr("Song identified"));
        ui->pixmapLabel->setPixmap(QPixmap(":/images/note.png"));
        ui->songButton->show();
        ui->songButton->setText(songDesc);
    }
}

ResultWindow::~ResultWindow()
{
    delete _song;
    delete ui;
}

void ResultWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(this->rect(),
                      QImage(":/images/bg.jpg"));
}

