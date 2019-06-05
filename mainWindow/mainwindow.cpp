#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    Qt::WindowStaysOnTopHint     窗口在最顶端，不会拖到任务栏下面
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setMouseTracking(true);

    //init
    title = new MainwindowTitle(this);
    suggestBox = new SuggestBox(this,title->getSearchLineEdit());
    currentMusicWidget = new CurrentMusicWidget(this);
    playStateWidget = new PlayStateWidget(this);
    playlistWidget = new PlayListWidget(this);

    //setup
    mouseLeftButtonPressed = false;
    resizeRegion = ResizeRegion::INVALID;
    ui->head->setWidget(title);
    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);
    ui->listofMusiclist->setup(this,ui->scrollArea,playlistWidget);
    ui->sliderPlay->setEnabled(false);

    loadData();
    connectSlots();

}

void MainWindow::loadData()
{
    musicpool = DataManager::loadMusicPool();
    ui->listofMusiclist->initialDefaultWidgets(musicpool);
    ui->listofMusiclist->loadUserMusiclists(musicpool);
    QList<Music*> play = DataManager::fromIDlist(musicpool,DataManager::loadMusiclist("正在播放"));
    QList<Music*> record = DataManager::fromIDlist(musicpool,DataManager::loadMusiclist("历史记录"));
    playlistWidget->setPlaylist(play,record);
}

void MainWindow::saveData()
{
    DataManager::saveMusicPool(musicpool);
    ui->listofMusiclist->saveLocalMusiclist();
    ui->listofMusiclist->saveUserMusiclist();
    playlistWidget->save();
}

MainWindow::~MainWindow()
{
    saveData();
    delete currentMusicWidget;
    delete suggestBox;
    delete playlistWidget;
    delete playStateWidget;
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

    int pos = this->size().height()-16-25-65;
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
        ui->btnPlay->setIcon(QIcon(":/image/resource/pause.png"));
    }
    else {
        playStateWidget->hide();
        if(state == QMediaPlayer::State::StoppedState){
            ui->sliderPlay->setEnabled(false);
        }
        ui->btnPlay->setIcon(QIcon(":/image/resource/play.png"));
    }
}

void MainWindow::connectSlots()
{
    FindMusicWidget *findMusicWidget =  qobject_cast<FindMusicWidget*>(ui->listofMusiclist->name_widgetHash.value("发现音乐"));
    LocalMusicWidget *localMusicWidget = qobject_cast<LocalMusicWidget*>(ui->listofMusiclist->name_widgetHash.value("本地音乐"));
    UserMusicWidget *favourateMusicWidget = qobject_cast<UserMusicWidget*>(ui->listofMusiclist->name_widgetHash.value("我喜欢的音乐"));
    connect(ui->sliderVolumn,SIGNAL(valueChanged(int)),
            playlistWidget,SLOT(changeVolume(int)));
    connect(suggestBox,SIGNAL(selectSearchedSong(Music*)),
            playlistWidget,SLOT(addMusicAndPlay(Music*)));
    connect(playlistWidget->list,&MusicTableWidget::sizeChanged,
            this,&MainWindow::setPlaylistBtnTextbycurSize);
    connect(ui->listofMusiclist,&MusiclistListWidget::sizeChanged,
            this,&MainWindow::resizeSubwidget);

    connect(ui->listofMusiclist,&MusiclistListWidget::listChanged,
            playlistWidget->list,&MusicTableWidget::buildBookMenu);
    connect(ui->listofMusiclist,&MusiclistListWidget::listChanged,
            playlistWidget->recordlist,&MusicTableWidget::buildBookMenu);

    connect(playlistWidget,&PlayListWidget::currentMediaChanged,
            playStateWidget,&PlayStateWidget::changeCurrentDisplay);

    connect(playlistWidget->list,&MusicTableWidget::musicDoubleClicked,
            playlistWidget,&PlayListWidget::changeListAndPlay);
    connect(playlistWidget->recordlist,&MusicTableWidget::musicDoubleClicked,
            playlistWidget,&PlayListWidget::changeListAndPlay);

    connect(playStateWidget,&PlayStateWidget::likeMusic,
            favourateMusicWidget,&UserMusicWidget::prepend);
    connect(playStateWidget,&PlayStateWidget::dislikeMusic,
            favourateMusicWidget,&UserMusicWidget::remove);

    connect(title,&MainwindowTitle::userWantExit,
            this,&MainWindow::aboutToExit);

    connect(suggestBox,&SuggestBox::selectSearchedSong,
            this,&MainWindow::onAddMusic);
    connect(ui->listofMusiclist,&MusiclistListWidget::addMusic,
            this,&MainWindow::onAddMusic);
    connect(playStateWidget,&PlayStateWidget::curPictureClicked,
            this,&MainWindow::showCurMusicInfo);
}

void MainWindow::setPlaylistBtnTextbycurSize(int size){
    ui->btnOpenPlaylist->setText(QString::number(size));
}

void MainWindow::aboutToExit()
{
    close();
}

void MainWindow::onAddMusic(Music *music)
{
    if(musicpool.contains(music->ID))
        return;
    musicpool.insert(music->ID,music);
}

void MainWindow::showCurMusicInfo(Music *music)
{
    currentMusicWidget->requestPicture(music->pic);
    currentMusicWidget->requestLrc(music->lrc);
    ui->scrollArea->takeWidget();
    ui->scrollArea->setWidget(currentMusicWidget);
    currentMusicWidget->show();
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        mouseLeftButtonPressed = true;
        mouseLastGlobalPos = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    mouseLeftButtonPressed = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouseGlobalPos = event->globalPos();
    if(mouseLeftButtonPressed){
        QRect size = geometry();
        QPoint sizeChangedValue = mouseGlobalPos-mouseLastGlobalPos;
        switch(resizeRegion){
        case ResizeRegion::BottomLeft:
            size.setBottomLeft(size.bottomLeft()+sizeChangedValue);
            break;
        case ResizeRegion::BottomRight:
            size.setBottomRight(size.bottomRight()+sizeChangedValue);
            break;
        default:
            return;
        }
        setGeometry(size);
        mouseLastGlobalPos = mouseGlobalPos;
        return;
    }
    if(inSquare(geometry().bottomRight(),mouseGlobalPos,10)){
        resizeRegion = ResizeRegion::BottomRight;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(inReverseSquare(mouseGlobalPos,geometry().bottomLeft(),5)){
        resizeRegion = ResizeRegion::BottomLeft;
        setCursor(Qt::SizeBDiagCursor);
    }
    else{
        resizeRegion = ResizeRegion::INVALID;
        setCursor(Qt::ArrowCursor);
        return;
    }
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


