#include <QAudioEncoderSettings>
#include <QUrl>

#include "recordermobility.h"

RecorderMobility::RecorderMobility(QObject *parent, const QString &file) :
    QObject(parent),
    _file(file)
{
        audiosource = new QAudioCaptureSource(this);
        capture = new QMediaRecorder(audiosource, this);

        audiosource->setAudioInput(audiosource->defaultAudioInput());

        // These settings are hardcoded for codegen library
        QAudioEncoderSettings settings;
        settings.setCodec("audio/PCM");
        settings.setChannelCount(1);
        settings.setSampleRate(11025);
        settings.setBitRate(350000);
        settings.setQuality(QtMultimediaKit::NormalQuality);
        settings.setEncodingMode(QtMultimediaKit::ConstantBitRateEncoding);

        capture->setOutputLocation(QUrl(_file));
        capture->setEncodingSettings(settings, QVideoEncoderSettings(), "wav");

        connect(capture, SIGNAL(durationChanged(qint64)),
                this, SIGNAL(durationChanged(qint64)));
        connect(capture, SIGNAL(error(QMediaRecorder::Error)),
                this, SIGNAL(error(QMediaRecorder::Error)));
}

RecorderMobility::~RecorderMobility()
{
    delete capture;
    delete audiosource;
}

void RecorderMobility::record()
{
    capture->record();
}

void RecorderMobility::stop()
{
    capture->stop();
}

QMediaRecorder::Error RecorderMobility::error()
{
    return capture->error();
}

QString RecorderMobility::errorString()
{
    return capture->errorString();
}
