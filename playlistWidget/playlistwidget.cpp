#include "playlistwidget.h"
#include "ui_playlistwidget.h"

PlayListWidget::PlayListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayListWidget)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playlist.setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(&playlist);
    connect(player,SIGNAL(positionChanged(qint64)),parent,SLOT(onPlayerPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),parent,SLOT(onPlayerDurationChanged(qint64)));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),parent,SLOT(onPlayerStateChanged(QMediaPlayer::State)));
    connect(player,SIGNAL(volumeChanged(int)),parent,SLOT(onPlayerVolumnChanged(int)));
}

PlayListWidget::~PlayListWidget()
{
    delete player;
    delete ui;
}


void PlayListWidget::addMedia(Music *music)
{
    QUrl url = music->url;
    QMediaContent mediaContent(url);
    playlist.addMedia(mediaContent);

}

void PlayListWidget::setPlaylist(QList<Music *> playlist)
{
    this->playlist.clear();
    foreach(Music* music,playlist){
        addMedia(music);
        int row = ui->playlistWidget->rowCount();
        ui->playlistWidget->insertRow(row);
        QTableWidgetItem *name = new QTableWidgetItem(music->name);
        QTableWidgetItem *author = new QTableWidgetItem(music->author);
        QTableWidgetItem *duration = new QTableWidgetItem(music->duration);
        ui->playlistWidget->setItem(row,0,name);
        ui->playlistWidget->setItem(row,1,author);
        ui->playlistWidget->setItem(row,2,duration);
    }
}

void PlayListWidget::setCurMedia(int index)
{
    playlist.setCurrentIndex(index);
}


void PlayListWidget::play()
{
    player->play();
}

void PlayListWidget::setMode(Playmode mode)
{
    QMediaPlaylist::PlaybackMode qmode;
    switch (mode) {
    case Playmode::loop:
        qmode = QMediaPlaylist::PlaybackMode::Loop;
        break;
    case Playmode::single:
        qmode = QMediaPlaylist::PlaybackMode::CurrentItemInLoop;
        break;
    case Playmode::random:
        qmode = QMediaPlaylist::PlaybackMode::Random;
        break;
    }
    playlist.setPlaybackMode(qmode);
}

void PlayListWidget::pause()
{
    player->pause();
}

void PlayListWidget::next()
{

    playlist.next();
}

void PlayListWidget::previous()
{
    playlist.previous();
}

bool PlayListWidget::isPlaying()
{
    qDebug()<<player->state();
    return (player->state()==QMediaPlayer::PlayingState);
}

void PlayListWidget::onSliderPlayPositonChanged(int pos)
{
    player->setPosition(pos);
}

void PlayListWidget::onSliderVolumePositionChanged(int volume)
{
    player->setVolume(volume);
}


