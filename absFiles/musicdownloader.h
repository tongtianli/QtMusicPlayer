#ifndef MUSICDOWNLOADER_H
#define MUSICDOWNLOADER_H

#include "music.h"

class MusicDownloader : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloader(QObject *parent = nullptr);
    void downloadMusic(Music *music);


private slots:
    void httpFinished();
    void httpReadyRead();

private:
    void startRequest(const QUrl &requestedUrl);
    QFile *openFileForWrite(const QString &fileName);

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    bool httpRequestAborted;

signals:

public slots:
};

#endif // MUSICDOWNLOADER_H
