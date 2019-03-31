

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    musicListWidget = new PlayListWidget(this);
    localMusicWidget = new LocalMusicWidget(this);
    playStateWidget = new PlayStateWidget(this);

    ui->scrollArea->setWidget(localMusicWidget);
    localMusicWidget->show();
    musicListWidget->hide();
    playStateWidget->hide();

    defaultMusicPool.load();
    connectSlots();

    localMusicWidget->load();

}

MainWindow::~MainWindow()
{
    defaultMusicPool.save();
    delete musicListWidget;
    delete localMusicWidget;
    delete playStateWidget;
    delete ui;
}


void MainWindow::on_btnOpenPlaylist_clicked()
{
    if(musicListWidget->isHidden()){
        QSize size = this->size();
        int width = size.width();
        int height = size.height();
        int marginx2 = 16;
        int playlistHeight = 400;
        int playlistWidth = 300;
        int posx = width-playlistWidth;
        int posy = height-playlistHeight-marginx2-25;
        musicListWidget->setGeometry(posx,posy,playlistWidth,playlistHeight);
        musicListWidget->show();
        musicListWidget->setFocus();
    }
    else musicListWidget->hide();
}

void MainWindow::allocateNewID(Music *music){
    music->ID = defaultMusicPool.size();
}

void MainWindow::refreshLabelPlayState()
{
    ui->labelPlayState->setText(posiText + ":" + duraText);
}


void MainWindow::on_btnPlay_clicked()
{
    int posy = this->size().height()-16-25-100;
    playStateWidget->setGeometry(0,posy,200,100);
//    if(player->isPlaying()){
//        player->pause();
//        playStateWidget->hide();
//    }else {
//        player->play();
//        playStateWidget->show();
//    }
}

void MainWindow::addMusicToPool(Music *music)
{
    if(music->ID==MUSIC::NOT_ALLOCATE) allocateNewID(music);
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

void MainWindow::onPositionChanged(int pos, QString posiText)
{
    ui->sliderPlay->setValue(pos);

    this->posiText = posiText;
    refreshLabelPlayState();
}

void MainWindow::onMaxPositionChanged(QString duraText)
{
    this->duraText = duraText;
    refreshLabelPlayState();
}



void MainWindow::connectSlots()
{
    connect(localMusicWidget,SIGNAL(localWidgetAddMusic(Music*)),this,SLOT(addMusicToPool(Music*)));
    connect(localMusicWidget,SIGNAL(localWidgetLoadList(QList<int>)),this,SLOT(initLocalWidget(QList<int>)));
//    connect(this,SIGNAL(sliderPlayPositionChanged(int)),player,SLOT(onSliderPlayPositonChanged(int)));
//    connect(player,SIGNAL(positionChanged(int,QString)),this,SLOT(onPositionChanged(int,QString)));
//    connect(player,SIGNAL(maxPositionChanged(QString)),this,SLOT(onMaxPositionChanged(QString)));
}

void MainWindow::on_sliderPlay_sliderReleased()
{
    int pos = ui->sliderPlay->sliderPosition();
    emit sliderPlayPositionChanged(pos);
}
