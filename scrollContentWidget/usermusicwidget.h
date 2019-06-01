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

    void prepend(Music *music);
    void remove(Music *music);

    void setName(QString name);
    QString name();
    void setInitDate(QString dateText);
    void load();

private slots:
    void on_playAllBtn_clicked();
    void onTableSizeChanged(int size);
    void setPicture(QNetworkReply *reply);

    void on_downloadAllBtn_clicked();

private:
    Ui::UserMusicWidget *ui;
    QString listname;
    QNetworkAccessManager manager;
    QPixmap defaultPix;
    MusicDownloader download;

};

#endif // USERMUSICWIDGET_H
