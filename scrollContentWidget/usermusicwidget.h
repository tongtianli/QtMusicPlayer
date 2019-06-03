#ifndef USERMUSICWIDGET_H
#define USERMUSICWIDGET_H

#include "music.h"
#include "musictablewidget.h"
#include "musicdownloader.h"


namespace Ui {
class UserMusicWidget;
}

class UserMusicWidget : public QWidget
{
    Q_OBJECT

public:
    MusicTableWidget *table;

    explicit UserMusicWidget(QWidget *parent = nullptr);
    ~UserMusicWidget();
    void setName(QString name);
    QString name();
    void setInitDate(QString dateText);
    void setList(QHash<int,Music*> musicpool, QList<int> idlist);
    void requestPicture(QUrl url);

signals:
    void addMusic(Music* music);

public slots:
    void prepend(Music *music);
    void remove(Music *music);

private slots:
    void on_playAllBtn_clicked();
    void onTableSizeChanged(int size);
    void setPicture(QNetworkReply *reply);

    void on_downloadAllBtn_clicked();


private:
    Ui::UserMusicWidget *ui;
    QString listname;
    QNetworkAccessManager manager;
    QNetworkAccessManager setup;
    QPixmap defaultPix;
    MusicDownloader download;

};

#endif // USERMUSICWIDGET_H
