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
    setAttribute(Qt::WA_Maemo5AutoOrientation);
#endif

    connect(QApplication::desktop(), SIGNAL(resized(int)),
            this, SLOT(orientationChanged()));
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
    orientationChanged();
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

void ResultWindow::orientationChanged()
{
#ifdef Q_WS_MAEMO_5
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    // Landscape
    if (screenGeometry.width() > screenGeometry.height())
    {
        ui->statusLabel->move(220, 40);
        ui->statusLabel->resize(540, 90);
        ui->pixmapLabel->move(70, 20);
        ui->songButton->move(90, 170);
        ui->songButton->resize(630, 90);
    }
    else // Portrait
    {
        ui->statusLabel->move(20, 130);
        ui->statusLabel->resize(440, 160);
        ui->pixmapLabel->move(30, 30);
        ui->songButton->move(20, 280);
        ui->songButton->resize(440, 120);
    }
#endif
}
