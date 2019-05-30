#include "musicdownloader.h"

MusicDownloader::MusicDownloader(QObject *parent) : QObject(parent)
{

}

void MusicDownloader::downloadMusic(Music *music)
{
    QString downloadDirectory = QDir::currentPath()+"/download";
    QString fileName = QDir::currentPath()+"/download/"+music->name+".mp3";
    if(QFile::exists(fileName)){
        if (QMessageBox::question(nullptr, QString("重新下载"),
                                  QString("音乐文件已存在%1. 是否重新下载?").arg(fileName),
                                  QString("是"),
                                  QString("否")
                                  )
            == QMessageBox::No) {
            return;
        }
        QFile::remove(fileName);
    }
    file = openFileForWrite(fileName);
    if(!file)
        return;
    startRequest(music->url);
}

void MusicDownloader::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;
    httpRequestAborted = false;

    reply = qnam.get(QNetworkRequest(url));
    connect(reply,&QNetworkReply::finished,this,&MusicDownloader::httpFinished);
    connect(reply,&QIODevice::readyRead,this,&MusicDownloader::httpReadyRead);

    qDebug()<<"Downloading "<<url;
}

QFile *MusicDownloader::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        qDebug()<<"文件打开失败";
        return nullptr;
    }
    return file.take();
}

void MusicDownloader::httpFinished()
{
    QFileInfo fi;
    if (file) {
        fi.setFile(file->fileName());
        file->close();
        delete file;
        file = nullptr;
    }
    if (httpRequestAborted) {
        reply->deleteLater();
        reply = nullptr;
        return;
    }
    if (reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        reply->deleteLater();
        reply = nullptr;
        return;
    }
    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    reply->deleteLater();
    reply=nullptr;
    if(!redirectionTarget.isNull()){
        const QUrl redirectedUrl = url.resolved(redirectionTarget.toUrl());
        file = openFileForWrite(fi.absoluteFilePath());
        startRequest(redirectedUrl);
        return;
    }
    qDebug()<<"Downloaded "<<fi.size()<<" bytes to "<<fi.fileName()<<"\nin\n"<<QDir::toNativeSeparators(fi.absolutePath());
}

void MusicDownloader::httpReadyRead()
{
    if (file)
        file->write(reply->readAll());
}
