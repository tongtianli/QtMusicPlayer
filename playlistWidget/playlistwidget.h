#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include "music.h"
#include "musictablewidget.h"
#include "datamanager.h"

namespace Ui{
class PlayListWidget;
}
class PlayListWidget : public QWidget
{
    Q_OBJECT

public:
    MusicTableWidget *list;
    MusicTableWidget *recordlist;

    explicit PlayListWidget(QWidget *parent = nullptr);
    ~PlayListWidget();

    void addMedia(Music* music);
    void setPlaylist(QList<Music*> playlist);
    void setCurMedia(int index);
    void insertMedia(int index, Music *music);
    void play();
    void setMode(Playmode mode);
    void pause();
    void next();
    void previous();
    bool isPlaying();
    void save();
    void setPlaylist(QList<Music *> play, QList<Music *> record);

signals:
    void maxPositionChanged(QString duration);
    void positionChanged(int pos,QString position);
    void currentMediaChanged(Music *music);


public slots:
    void changePlayPosition(int pos);
    void changeVolume(int volume);
    void addMusicAndPlay(Music* music);
    void changeListAndPlay(QList<Music*> musiclist,int index);
    void playListLater(QList<Music*> musiclist);

private:
    const int maxRecordNum = 100;

    QWidget* parent;
    Ui::PlayListWidget *ui;
    QMediaPlaylist *playlist;
    QMediaPlayer *player;

    void connectSlots();

private slots:
    void onCurPlaylistIndexChanged(int index);
    void on_btnCleanList_clicked();
    void on_btnCleanRecordlist_clicked();
};
#endif // PLAYLISTWIDGET_H
