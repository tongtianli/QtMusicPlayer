#ifndef FINDMUSICWIDGET_H
#define FINDMUSICWIDGET_H

#include "music.h"
#include "playlist.h"

namespace Ui {
class FindMusicWidget;
}


class FindMusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindMusicWidget(QWidget *parent = nullptr);
    ~FindMusicWidget();

private slots:
    void on_searchBox_returnPressed();
    void handleNetworkData(QNetworkReply *networkReply);

    void on_tabWidget_currentChanged(int index);

    void on_btnSearch_clicked();

private:
    Ui::FindMusicWidget *ui;
    QString searchUrl;
    QString searchContect;
    QList<Playlist*> playlistCache;
    QList<Music*> musicCache;


    QNetworkAccessManager networkManager;

    void connectSlots();
    void sendRequest();
    void showResult();
};

#endif // FINDMUSICWIDGET_H
