#ifndef ECHONESTSONG_H
#define ECHONESTSONG_H

#include <QObject>

class EchoNestSong : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString artist_name READ artist WRITE setArtist)
    Q_PROPERTY(QString artist_id READ artistId WRITE setArtist)

public:
    EchoNestSong(QObject * parent = 0);
    ~EchoNestSong();

    QString id() const { return _id; }
    QString title() const { return _title; }
    QString artist() const { return _artist; }
    QString artistId() const { return _artistId; }
    void setId(const QString &id) { _id = id.trimmed(); }
    void setTitle(const QString &title) { _title = title.trimmed(); }
    void setArtist(const QString &artist) { _artist = artist.trimmed(); }
    void setArtistId(const QString &artistId) { _artistId = artistId.trimmed(); }

private:
    QString _id;
    QString _title;
    QString _artist;
    QString _artistId;
};

#endif // ECHONESTSONG_H
