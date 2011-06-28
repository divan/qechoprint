#ifndef ECHONESTREQUEST_H
#define ECHONESTREQUEST_H

#include <QObject>
#include "echonestsong.h"

#include <QVariantMap>
#include <QNetworkAccessManager>

class EchoNestRequest : public QObject
{
    Q_OBJECT
public:
    explicit EchoNestRequest(QObject *parent = 0,
                             QNetworkAccessManager * nam = 0);
    explicit EchoNestRequest(QObject *parent = 0,
                             QNetworkAccessManager * nam = 0,
                             QString apiKey = "",
                             QString codegenVersion = "");
    ~EchoNestRequest();

    void setAPIKey(const QString &apiKey);
    void setCodegenVersion(const QString &codegenVersion);

    EchoNestSong * identifySong(QString code);

signals:

public slots:

private:
    QNetworkAccessManager * _nam;

    QString _apiKey;
    QString _codegenVersion;

    QVariantMap requestMethod(const QUrl &url);
};

#endif // ECHONESTREQUEST_H
