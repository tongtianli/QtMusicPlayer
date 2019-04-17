

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    suggestBox = new SuggestBox(this,ui->searchBox);
    playlistWidget = new PlayListWidget(this);
    localMusicWidget = new LocalMusicWidget(this);
    playStateWidget = new PlayStateWidget(this);
    favoriteMusicWidget = new UserMusicWidget(this);
    favoriteMusicWidget->setTableName("我喜爱的音乐");
    ui->scrollArea->setWidget(localMusicWidget);
    ui->splitter->setStretchFactor(1,1);
    localMusicWidget->show();
    connectSlots();
    ui->sliderPlay->setEnabled(false);
    ui->labelPlayState->setText("");
    localMusicWidget->setProperty("centralWidget","true");
    favoriteMusicWidget->setProperty("centralWidget","true");
    ui->listofMusiclist->setCurrentRow(1);
}

MainWindow::~MainWindow()
{
    delete suggestBox;
    delete playlistWidget;
    delete localMusicWidget;
    delete playStateWidget;
    delete favoriteMusicWidget;
    delete ui;
}

void MainWindow::resizeSubwidget(){
    int width = this->width();
    int height = this->height();
    int playlistHeight = playlistWidget->height();
    int playlistWidth = playlistWidget->height();
    int posx = width-playlistWidth;
    int posy = height-playlistHeight-bottomHeight;
    playlistWidget->setGeometry(posx,posy,playlistWidth,playlistHeight);

    int pos = this->size().height()-16-25-100;
    playStateWidget->setGeometry(0,pos,ui->listofMusiclist->size().width(),100);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
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
        ui->sliderPlay->setEnabled(true);
    }
    else {
        playStateWidget->hide();
        if(state == QMediaPlayer::State::StoppedState){
            ui->sliderPlay->setEnabled(false);
        }
    }
}



void MainWindow::connectSlots()
{
    connect(ui->sliderVolumn,SIGNAL(valueChanged(int)),playlistWidget,SLOT(changeVolume(int)));
    connect(suggestBox,SIGNAL(selectSearchedSong(Music*)),playlistWidget,SLOT(addMusicAndPlay(Music*)));
    connect(localMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
    connect(playlistWidget->list,&MusicTableWidget::sizeChanged,this,&MainWindow::setPlaylistBtnTextbycurSize);

}

void MainWindow::setPlaylistBtnTextbycurSize(int size){
    ui->btnOpenPlaylist->setText(QString::number(size));
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



void MainWindow::on_btnExit_clicked()
{
    this->close();
}

void MainWindow::on_btnFullScreen_clicked()
{
    this->isMaximized() ? this->showNormal() : this->showMaximized();
}

void MainWindow::on_btnHide_clicked()
{
    this->showMinimized();
}
