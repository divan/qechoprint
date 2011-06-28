#ifndef RECORDERMOBILITY_H
#define RECORDERMOBILITY_H

#include <QObject>
#include <QMediaRecorder>
#include <QAudioCaptureSource>

class RecorderMobility : public QObject
{
    Q_OBJECT
public:
    explicit RecorderMobility(QObject *parent = 0, const QString &file = "");
    ~RecorderMobility();

    void record();
    void stop();
    QMediaRecorder::Error error();
    QString errorString();

private:
    QString _file; // file to be written
    // When I find out it's possible to record
    // to the memory with QMediaRecorder, I'll
    // get rid of this file stuff

public slots:

private:
    QAudioCaptureSource* audiosource;
    QMediaRecorder* capture;

signals:
    void durationChanged(qint64);
    void error(QMediaRecorder::Error);
};

#endif // RECORDERMOBILITY_H
