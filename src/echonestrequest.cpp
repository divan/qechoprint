#include <QtNetwork>
#include <qjson/parser.h>
#include <qjson/qobjecthelper.h>

#include <QNetworkAccessManager>
#include "echonestrequest.h"

const QString ECHONEST_API_URL = "http://developer.echonest.com/api/v4/";

EchoNestRequest::EchoNestRequest(QObject *parent,
                                 QNetworkAccessManager *nam) :
    QObject(parent),
    _nam(nam)
{
}

EchoNestRequest::~EchoNestRequest()
{
}

EchoNestRequest::EchoNestRequest(QObject *parent,
                                 QNetworkAccessManager *nam,
                                 QString apiKey,
                                 QString codegenVersion) :
    QObject(parent),
    _nam(nam)
{
    setAPIKey(apiKey);
    setCodegenVersion(codegenVersion);
}

void EchoNestRequest::setAPIKey(const QString &apiKey)
{
    _apiKey = apiKey;
}

void EchoNestRequest::setCodegenVersion(const QString &codegenVersion)
{
    _codegenVersion = codegenVersion;
}

using namespace QJson;

/*
 * Returns allocated EchoNestSong object.
 * Called must care to delete after using.
 */
EchoNestSong * EchoNestRequest::identifySong(QString code, int numCodes)
{
    QUrl url(ECHONEST_API_URL + "song/identify");
    url.addQueryItem("api_key", _apiKey);
    url.addQueryItem("version", _codegenVersion);
    url.addQueryItem("code", code);
    //url.addQueryItem("code_count", QString(numCodes));

    QVariantMap response = requestMethod(url);
    if (response.isEmpty())
        return NULL;

    QList<QVariant> songs = response["songs"].toList();
    if (songs.isEmpty())
        return NULL;

    EchoNestSong * song = new EchoNestSong;
    QVariantMap songDesc = songs.at(0).toMap();
    QObjectHelper::qvariant2qobject(songDesc, song);

    return song;
}

QVariantMap EchoNestRequest::requestMethod(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply * reply = _nam->get(request);

    QEventLoop loop;
    loop.connect(reply, SIGNAL(finished()), SLOT(quit()));
    loop.exec();

    bool ok;
    Parser parser;
    QVariantMap variant = parser.parse(reply->readAll(), &ok).toMap();
    QVariantMap response = variant["response"].toMap();
    // TODO: add error handling

    delete reply;

    return response;
}
