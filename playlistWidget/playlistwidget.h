#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include "music.h"

namespace Ui{
class PlayListWidget;
}
class PlayListWidget : public QWidget
{
    Q_OBJECT

public:
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
    void onSliderPlayPositonChanged(int pos);
    void onSliderVolumePositionChanged(int volume);
    void addMusicAndPlay(Music* music);

private:
    QWidget* parent;
    Ui::PlayListWidget *ui;
    void refreshMediaInfo();
    void refreshRandomlist();
    void connectSlots();
    QMediaPlaylist *playlist;
    QMediaPlayer *player = new QMediaPlayer(this);
    void insertToPlaylistWidget(int index, Music *music);
    QList<Music*> musicList;
    int maxRecordNum = 100;
    void clearTable(int listIndex);

private slots:
    void onCurPlaylistIndexChanged(int index);
    void on_btnCleanList_clicked();
};
#endif // PLAYLISTWIDGET_H
