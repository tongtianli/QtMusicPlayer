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
    list->setName("正在播放");
    list->setColumnWidth(0,200);
    recordlist->setName("历史记录");
    recordlist->setColumnWidth(0,200);
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
    QMediaContent mediaContent(music->url);
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

void PlayListWidget::insertMedia(int index, Music *music)
{
    QMediaContent mediaContent(music->url);
    playlist->insertMedia(index,mediaContent);
    list->insertMusic(index,music);
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
    int row = list->currentRow()+1;
    list->setCurrentCell(row,0);
}

void PlayListWidget::previous()
{
    playlist->previous();
    int row = list->currentRow()-1;
    list->setCurrentCell(row,0);
}

bool PlayListWidget::isPlaying()
{
    return (player->state()==QMediaPlayer::PlayingState);
}

void PlayListWidget::save()
{
    DataManager::saveMusiclist("正在播放",ui->playlistWidget);
    DataManager::saveMusiclist("历史记录",ui->recordlist);
}

void PlayListWidget::setPlaylist(QList<Music*> play,QList<Music*> record)
{
    ui->playlistWidget->setMusiclist(play);
    ui->recordlist->setMusiclist(record);
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

void PlayListWidget::playListLater(QList<Music *> musiclist)
{
    int i = playlist->currentIndex();
    for(int j=0;j<musiclist.size();j++){
        insertMedia(++i,musiclist.at(j));
    }
}

void PlayListWidget::onCurPlaylistIndexChanged(int index)
{
    //将历史纪录100条以后的删去
    while(recordlist->rowCount()>=maxRecordNum)
        recordlist->remove(maxRecordNum);

    //加入历史记录
    Music *music = list->get(index);
    recordlist->insertMusic(0,music);

    //清除之前的记录
    for(int i=1;i<ui->recordlist->rowCount();i++){
        if(recordlist->get(i)==music)
        {
            recordlist->remove(i);
            break;
        }
    }

    emit(currentMediaChanged(music));
}

void PlayListWidget::on_btnCleanList_clicked()
{
    list->clearAll();
    player->stop();
    playlist->clear();
}

void PlayListWidget::on_btnCleanRecordlist_clicked()
{
    recordlist->clearAll();
}
