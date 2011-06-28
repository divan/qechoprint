#ifndef CODEGENWRAPPER_H
#define CODEGENWRAPPER_H

#include <QObject>
#include <QString>
#include <codegen/Codegen.h>
#include "codegen/AudioStreamInput.h"

class CodeGenWrapper : public QObject
{
    Q_OBJECT
public:
    explicit CodeGenWrapper(QObject *parent = 0,
                            const QString &filename = "",
                            const uint length = 0);
    ~CodeGenWrapper();

signals:

public slots:

private:
    QString getCode(const float * samples, const uint count);

    const QString _filename;
    const uint _length;
    FfmpegStreamInput * _audio;

private slots:
    void startCodegen();

signals:
   void codeReady(const QString &code);
};

#endif // CODEGENWRAPPER_H
