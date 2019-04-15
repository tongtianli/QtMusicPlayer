#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include "music.h"
#include "musictablewidget.h"

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
    void play();
    void setMode(Playmode mode);
    void pause();
    void next();
    void previous();
    bool isPlaying();

signals:
    void maxPositionChanged(QString duration);
    void positionChanged(int pos,QString position);


public slots:
    void changePlayPosition(int pos);
    void changeVolume(int volume);
    void addMusicAndPlay(Music* music);
    void changeListAndPlay(QList<Music*> musiclist,int index);

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
};
#endif // PLAYLISTWIDGET_H
