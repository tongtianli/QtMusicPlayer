#include "playlistwidget.h"
#include "ui_playlistwidget.h"

PlayListWidget::PlayListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayListWidget)
{
    ui->setupUi(this);
    this->parent = parent;
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    list = ui->playlistWidget;
    recordlist = ui->recordlist;
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    connectSlots();
    this->hide();
}

PlayListWidget::~PlayListWidget()
{
    delete player;
    delete playlist;
    delete ui;
}

void PlayListWidget::connectSlots()
{
    connect(player,SIGNAL(positionChanged(qint64)),
            parent,SLOT(onPlayerPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),
            parent,SLOT(onPlayerDurationChanged(qint64)));
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),
            parent,SLOT(onPlayerStateChanged(QMediaPlayer::State)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),
            this,SLOT(onCurPlaylistIndexChanged(int)));
    connect(parent,SIGNAL(sliderPlayPositionChanged(int)),
            this,SLOT(changePlayPosition(int)));
}


void PlayListWidget::addMedia(Music *music)
{
    QUrl url = music->url;
    QMediaContent mediaContent(url);
    playlist->addMedia(mediaContent);
    list->append(music);
}

void PlayListWidget::setPlaylist(QList<Music *> playlist)
{
    this->playlist->clear();
    list->clearAll();
    foreach(Music* music,playlist){
        addMedia(music);
    }
}

void PlayListWidget::setCurMedia(int index)
{
    playlist->setCurrentIndex(index);
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
    playlist->setPlaybackMode(qmode);
}

void PlayListWidget::pause()
{
    player->pause();
}

void PlayListWidget::next()
{
    playlist->next();
}

void PlayListWidget::previous()
{
    playlist->previous();
}

bool PlayListWidget::isPlaying()
{
    qDebug()<<player->state();
    return (player->state()==QMediaPlayer::PlayingState);
}

void PlayListWidget::changePlayPosition(int pos)
{
    player->setPosition(pos);
}

void PlayListWidget::changeVolume(int volume)
{
    player->setVolume(volume);
}

void PlayListWidget::addMusicAndPlay(Music *music)
{
    int index = playlist->currentIndex();
    index = index<0?0:index;
    list->insertMusic(index,music);
    QMediaContent media(music->url);
    playlist->insertMedia(index,media);
    playlist->setCurrentIndex(index);
    player->play();
}

void PlayListWidget::changeListAndPlay(QList<Music *> musiclist, int index)
{
    setPlaylist(musiclist);
    setCurMedia(index);
    play();
}

void PlayListWidget::onCurPlaylistIndexChanged(int index)
{
    while(ui->recordlist->rowCount()>=maxRecordNum)
        ui->recordlist->removeRow(ui->recordlist->rowCount()-1);
    ui->recordlist->insertRow(0);
    Music *music = list->get(index);
    QTableWidgetItem *name = new QTableWidgetItem(music->name);
    QTableWidgetItem *author = new QTableWidgetItem(music->singer);
    QTableWidgetItem *playTime = new QTableWidgetItem(QTime::currentTime().toString());
    qDebug()<<name->text()<<author->text()<<playTime->text();
    ui->recordlist->setItem(0,0,name);
    ui->recordlist->setItem(0,1,author);
    ui->recordlist->setItem(0,2,playTime);
    for(int i=1;i<ui->recordlist->rowCount();i++){
        if(ui->recordlist->item(i,0)->text()==music->name&&ui->recordlist->item(i,1)->text()==music->singer)
        {
            ui->recordlist->removeRow(i);
            break;
        }
    }
}

void PlayListWidget::on_btnCleanList_clicked()
{
    if(ui->tabWidget->currentIndex() ==
            ui->tabWidget->indexOf(ui->playlistWidget)){     //当前在播放列表
        list->clearAll();
        player->stop();
        playlist->clear();
    }else{                                      //当前在历史记录
        recordlist->clearAll();
    }
}
