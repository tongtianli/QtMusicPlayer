

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    playlistWidget = new PlayListWidget(this);
    localMusicWidget = new LocalMusicWidget(this);
    playStateWidget = new PlayStateWidget(this);

    ui->scrollArea->setWidget(localMusicWidget);
    localMusicWidget->show();
    playlistWidget->hide();
    playStateWidget->hide();

    defaultMusicPool.load();
    connectSlots();

    localMusicWidget->load();

}

MainWindow::~MainWindow()
{
    defaultMusicPool.save();
    delete playlistWidget;
    delete localMusicWidget;
    delete playStateWidget;
    delete ui;
}

void MainWindow::connectSlots()
{
    connect(localMusicWidget,SIGNAL(localWidgetAddMusic(Music*)),this,SLOT(addMusicToPool(Music*)));
    connect(localMusicWidget,SIGNAL(localWidgetLoadList(QList<int>)),this,SLOT(initLocalWidget(QList<int>)));
    connect(this,SIGNAL(sliderPlayPositionChanged(int)),playlistWidget,SLOT(onSliderPlayPositonChanged(int)));
    connect(playlistWidget,SIGNAL(positionChanged(int,QString)),this,SLOT(onPositionChanged(int,QString)));
    connect(playlistWidget,SIGNAL(maxPositionChanged(QString)),this,SLOT(onMaxPositionChanged(QString)));
    connect(localMusicWidget,SIGNAL(playLocalMusiclist(int,QList<Music*>)),this,SLOT(onChangelistRequested(int,QList<Music*>)));
}

void MainWindow::on_btnOpenPlaylist_clicked()
{
    if(playlistWidget->isHidden()){
        QSize size = this->size();
        int width = size.width();
        int height = size.height();
        int marginx2 = 16;
        int playlistHeight = 400;
        int playlistWidth = 300;
        int posx = width-playlistWidth;
        int posy = height-playlistHeight-marginx2-25;
        playlistWidget->setGeometry(posx,posy,playlistWidth,playlistHeight);
        playlistWidget->show();
        playlistWidget->setFocus();
    }
    else playlistWidget->hide();
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
    if(playlistWidget->isPlaying()){
        playlistWidget->pause();
        playStateWidget->hide();
    }else {
        playlistWidget->play();
        playStateWidget->show();
    }
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

void MainWindow::onChangelistRequested(int index, QList<Music *> list)
{
    qDebug()<<"index:"<<index<<"list:"<<list;
    playlistWidget->setPlaylist(list);
    playlistWidget->setCurMedia(index);
    playlistWidget->play();
}

void MainWindow::on_sliderPlay_sliderReleased()
{
    int pos = ui->sliderPlay->sliderPosition();
    emit sliderPlayPositionChanged(pos);
}
