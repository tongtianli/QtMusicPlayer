

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    suggestBox = new SuggestBox(this,ui->searchBox);
    playlistWidget = new PlayListWidget(this);
    localMusicWidget = new LocalMusicWidget(this);
    playStateWidget = new PlayStateWidget(this);
    userMusicWidget = new UserMusicWidget(this);

    ui->scrollArea->setWidget(localMusicWidget);
    localMusicWidget->show();
    defaultMusicPool.load();
    localMusicWidget->load();

    connectSlots();

}

MainWindow::~MainWindow()
{
    defaultMusicPool.save();
    delete suggestBox;
    delete playlistWidget;
    delete localMusicWidget;
    delete playStateWidget;
    delete ui;
}

void MainWindow::resizeSubwidget(){
    QSize size = this->size();
    int width = size.width();
    int height = size.height();
    int marginx2 = 16;
    int playlistHeight = 400;
    int playlistWidth = 300;
    int posx = width-playlistWidth;
    int posy = height-playlistHeight-marginx2-25;
    playlistWidget->setGeometry(posx,posy,playlistWidth,playlistHeight);

    int pos = this->size().height()-16-25-100;
    playStateWidget->setGeometry(0,pos,200,100);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeSubwidget();
}

void MainWindow::onPlayerPositionChanged(qint64 position)
{
    int pos = int(position);
    if(ui->sliderPlay->isSliderDown()) return;
    ui->sliderPlay->setSliderPosition(pos);
    int secs = pos/1000;
    int mins = secs/60;
    secs = secs%60;
    posiText=QString("%1:%2").arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0'));
    ui->labelPlayState->setText(posiText+"/"+duraText);
}

void MainWindow::onPlayerDurationChanged(qint64 duration)
{
    int dur = int(duration);
    ui->sliderPlay->setMaximum(dur);
    int secs = dur/1000;
    int mins = secs/60;
    secs = secs%60;
    duraText = QString("%1:%2").arg(mins,2,10,QLatin1Char('0')).arg(secs,2,10,QLatin1Char('0'));
    ui->labelPlayState->setText(posiText+"/"+duraText);
}

void MainWindow::onPlayerStateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::State::PlayingState){
        resizeSubwidget();
        playStateWidget->show();
    }
    else {
        playStateWidget->hide();
    }
}



void MainWindow::connectSlots()
{
    connect(localMusicWidget,SIGNAL(localWidgetAddMusic(Music*)),this,SLOT(addMusicToPool(Music*)));
    connect(localMusicWidget,SIGNAL(localWidgetLoadList(QList<int>)),this,SLOT(initLocalWidget(QList<int>)));
    connect(this,SIGNAL(sliderPlayPositionChanged(int)),playlistWidget,SLOT(onSliderPlayPositonChanged(int)));
    connect(localMusicWidget,SIGNAL(playLocalMusiclist(int,QList<Music*>)),this,SLOT(onChangelistRequested(int,QList<Music*>)));
    connect(ui->sliderVolumn,SIGNAL(valueChanged(int)),playlistWidget,SLOT(onSliderVolumePositionChanged(int)));
    connect(suggestBox,SIGNAL(selectSearchedSong(Music*)),playlistWidget,SLOT(addMusicAndPlay(Music*)));

}

void MainWindow::on_btnOpenPlaylist_clicked()
{
    if(playlistWidget->isHidden()){
        resizeSubwidget();
        playlistWidget->show();
        playlistWidget->setFocus();
    }
    else playlistWidget->hide();
}


void MainWindow::on_btnPlay_clicked()
{
    if(playlistWidget->isPlaying()){
        playlistWidget->pause();
    }else {
        playlistWidget->play();
    }
}

void MainWindow::addMusicToPool(Music *music)
{
    defaultMusicPool.addMusic(music);
}


void MainWindow::initLocalWidget(QList<int> idlist)
{
    QList<Music*> list;
    foreach(int id,idlist){
        list.append(this->defaultMusicPool.getMusicByID(id));
        qDebug()<<list<<endl;
    }
    localMusicWidget->refreshTable(list);
}


void MainWindow::onChangelistRequested(int index, QList<Music *> list)
{
    qDebug()<<"index:"<<index<<"list:"<<list;
    ui->btnOpenPlaylist->setText(QString::number(list.size()));
    playlistWidget->setPlaylist(list);
    playlistWidget->setCurMedia(index);
    playlistWidget->play();
}

void MainWindow::on_sliderPlay_sliderReleased()
{
    int position = ui->sliderPlay->sliderPosition();
    emit sliderPlayPositionChanged(position);
}

void MainWindow::on_btnLast_clicked()
{
    playlistWidget->previous();
}

void MainWindow::on_btnNext_clicked()
{
    playlistWidget->next();
}

void MainWindow::on_listofMusiclist_itemClicked(QListWidgetItem *item)
{
    ui->scrollArea->setWidget(userMusicWidget);
}
