#include <QTimer>
#include <QDebug>

#include "codegenwrapper.h"

CodeGenWrapper::CodeGenWrapper(QObject *parent, const QString &filename, const uint length) :
    QObject(parent),
    _filename(filename),
    _length(length)
{
    _audio = new FfmpegStreamInput;
    QTimer::singleShot(10, this, SLOT(startCodegen()));
}

CodeGenWrapper::~CodeGenWrapper()
{
    delete _audio;
}

void CodeGenWrapper::startCodegen()
{
    QString code;
    try
    {
        _audio->ProcessFile(_filename.toLatin1().data(), 0, _length);
        code = getCode(_audio->getSamples(), _audio->getNumSamples());
    }
    catch (std::runtime_error)
    {
        qWarning() << "Decoding failed!";
    }

    emit(codeReady(code));
}

QString CodeGenWrapper::getCode(const float * samples, uint count)
{
    if (!count)
    {
        qWarning() << "Ffmpeg output is empty, aborting";
        return "";
    }
    Codegen * codegen = new Codegen(samples, count, 0);
    QString code = codegen->getCodeString().c_str();
    delete codegen;
#ifdef DEBUG
    qDebug() << "Code: " << code;
#endif
    return code;
}
