#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#ifdef USE_LIBECHONEST
#include <echonest/Config.h>
#include <echonest/Song.h>
#else
#include "echonestrequest.h"
#include "echonestsong.h"
#endif

#include <QDebug>

#include "resultwindow.h"
#include "codegenwrapper.h"

#ifdef Q_WS_MAEMO_5
    #include <QMaemo5InformationBox>
#endif

// Uncomment this to skip recording step
// and use prerecorded audio from TEMP_FILE
#define TEMP_DEV_MODE

#ifdef Q_WS_MAEMO_5
const QString DEFAULT_FILE = "/home/user/MyDocs/echoprint.wav";
const QString TEMP_FILE = "/home/user/MyDocs/out.wav";
#elif defined(Q_WS_X11)
const QString DEFAULT_FILE = "/tmp/echoprint.wav";
const QString TEMP_FILE = "/tmp/out.wav";
#elif defined(Q_WS_WIN)
// add something here if you're unlucky to
// stick with windows
#endif
const QString API_KEY = "IIBZZ0ZGCAKWHJNNB";
#ifdef TEMP_DEV_MODE
const int CAPTURE_TIME = 1;
#else
const int CAPTURE_TIME = 25;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    , _inProgress(false)
{
    // Preload icons for faster switching
    iconMicrophone = new QIcon(":/images/microphone.png");
    iconRecord = new QIcon(":/images/record.png");
    iconProcess = new QIcon(":/images/process.png");
    iconNetwork = new QIcon(":/images/network.png");

    ui->setupUi(this);
#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

    nam = new QNetworkAccessManager(this);

    recorder = new RecorderMobility(this, DEFAULT_FILE);
    connect(recorder, SIGNAL(durationChanged(qint64)),
            this, SLOT(updateProgress(qint64)));
    connect(recorder, SIGNAL(error(QMediaRecorder::Error)),
            this, SLOT(displayErrorMessage()));

    connect(this, SIGNAL(captureFinished()), this, SLOT(processAudio()));
    connect(this, SIGNAL(resultReady(const EchoNestSong *)),
            this, SLOT(showResult(const EchoNestSong *)));

#ifdef USE_LIBECHONEST
    Echonest::Config::instance()->setAPIKey(API_KEY.toLatin1());
#endif

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(stopCapture()));
}

MainWindow::~MainWindow()
{
    delete iconMicrophone;
    delete iconNetwork;
    delete iconProcess;
    delete iconRecord;
    delete nam;
    delete recorder;
    delete timer;
    delete ui;
}

void MainWindow::on_recordButton_clicked()
{
    if (!_inProgress)
    {
        _inProgress = true;
        ui->recordButton->setIcon(*iconRecord);
        ui->label->setText(tr("Recording %1 of %2 seconds...")
            .arg(1).arg(CAPTURE_TIME));
        repaint();

        recorder->record();
        ui->recordButton->setChecked(true);
        timer->start(CAPTURE_TIME*1000);
    }
    else
    {
        if (QMessageBox::question(this, tr("Confirmation"),
                                  tr("Do you really want to cancel recording?"),
                                  QMessageBox::Yes | QMessageBox::No)
                                  == QMessageBox::Yes)
        {
            timer->stop();
            recorder->stop();
            restoreInitialState();
        }

    }
}

void MainWindow::stopCapture(void)
{
    recorder->stop();
    emit(captureFinished());
}

void MainWindow::updateProgress(qint64 duration)
{
    if (recorder->error() != QMediaRecorder::NoError)
        return;

    ui->label->setText(tr("Recording %1 of %2 seconds...")
            .arg((duration)/1000 + 1).arg(CAPTURE_TIME));
}

void MainWindow::processAudio(void)
{
    ui->label->setText(tr("Processing data..."));
    ui->recordButton->setIcon(*iconProcess);
    repaint();

#ifdef TEMP_DEV_MODE
    CodeGenWrapper * codegen = new CodeGenWrapper(this, TEMP_FILE, 30);
#else
    CodeGenWrapper * codegen  = new CodeGenWrapper(this, DEFAULT_FILE, CAPTURE_TIME);
#endif /* TEMP_DEV_MODE */

    connect(codegen, SIGNAL(codeReady(QString)), this, SLOT(sendData(QString)));
}

void MainWindow::sendData(const QString &data)
{
    // Quick error checks before sending
    if (data.isEmpty())
    {
        showError(tr("Can't process audio ;("));
        return;
    }

    ui->label->setText(tr("Sending data..."));
    ui->recordButton->setIcon(*iconNetwork);
    repaint();

#ifdef USE_LIBECHONEST
    QNetworkReply * reply = Echonest::Song::identify(data);
    QEventLoop loop;
    loop.connect(reply, SIGNAL(finished()), SLOT(quit()));
    loop.exec();

    QByteArray answer(reply->readAll());
#else
    EchoNestRequest request(this, nam, API_KEY, QString("4.11"));
    EchoNestSong * song = request.identifySong(data);
#endif

    emit(resultReady(song));
}

void MainWindow::showResult(const EchoNestSong *song)
{
    ResultWindow * window = new ResultWindow(this, song);
    window->show();

    // Cleanup interface while user see result window
    restoreInitialState();
}

void MainWindow::restoreInitialState()
{
    ui->label->setText(tr("Press the button to recognize music"));
    ui->recordButton->setChecked(false);
    ui->recordButton->setIcon(*iconMicrophone);
    repaint();
    _inProgress = false;
}

void MainWindow::displayErrorMessage()
{
    showError(recorder->errorString());
}

void MainWindow::showError(const QString &errorString)
{
#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox::information(this,
                                    QString("<br /><br />%1<br/><br />").arg(errorString),
                                    QMaemo5InformationBox::NoTimeout);
#else
    QMessageBox::critical(this,
                          "Error",
                          errorString,
                          QMessageBox::Ok);
#endif
    restoreInitialState();
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto)
    {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702)
        {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(this->rect(),
                      QImage(":/images/bg.jpg"));
}

