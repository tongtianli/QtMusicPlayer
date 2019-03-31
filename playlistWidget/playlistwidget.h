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

private:
    Ui::PlayListWidget *ui;
    void refreshMediaInfo();
    void refreshRandomlist();

    QMediaPlaylist playlist;
    QMediaPlayer *player = new QMediaPlayer(this);
    QMediaContent curMedia;
private slots:

};
#endif // PLAYLISTWIDGET_H
