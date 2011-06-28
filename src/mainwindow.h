#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QNetworkAccessManager>

#include "echonestsong.h"
#include "recordermobility.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

private slots:
    void orientationChanged();
    void on_recordButton_clicked();

    void updateProgress(qint64 pos);
    void displayErrorMessage();

    void processAudio();
    void sendData(const QString &data);
    void showResult(const EchoNestSong * song);

    void paintEvent(QPaintEvent*);
    void stopCapture(void);

    void on_echoNestButton_clicked();

private:
    Ui::MainWindow *ui;

    QIcon * iconMicrophone;
    QIcon * iconRecord;
    QIcon * iconProcess;
    QIcon * iconNetwork;

    void restoreInitialState();
    void showError(const QString& errorString);

    bool _inProgress;
    QTimer * timer;

    QNetworkAccessManager * nam;
    RecorderMobility * recorder;

signals:
    void captureFinished(void);
    void resultReady(const EchoNestSong * song);
};

#endif // MAINWINDOW_H
