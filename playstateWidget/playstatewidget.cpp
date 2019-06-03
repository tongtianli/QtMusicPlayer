#include "playstatewidget.h"
#include "ui_playstatewidget.h"

PlayStateWidget::PlayStateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayStateWidget)
{
    ui->setupUi(this);
    this->hide();

    defaultPix = QPixmap(":/image/resource/localMusicPic.png").scaled(50,50);
    ui->picture->setPixmap(defaultPix);

    connect(&manager,&QNetworkAccessManager::finished,
            this, &PlayStateWidget::setPicture);
}

PlayStateWidget::~PlayStateWidget()
{
    delete ui;
}

void PlayStateWidget::changeCurrentDisplay(Music *music)
{
    QUrl picUrl = music->pic;
    if(picUrl.isValid()&&(!picUrl.isLocalFile())){
        manager.get(QNetworkRequest(picUrl));
    }
    else {
        //defaultPix = QPixmap(":/image/resource/localMusicPic.png").scaled(50,50);
        ui->picture->setPixmap(defaultPix);
    }
    ui->author->setText(music->singer);
    ui->songName->setText(music->name);
    this->curMusic = music;
    if(music->like){
        ui->likeBtn->setIcon(QIcon(":/image/resource/like_yes.png"));
    }
    else{
        ui->likeBtn->setIcon(QIcon(":/image/resource/like_no.png"));
    }
    ui->downloadBtn->setIcon(QIcon(":/image/resource/download.png"));
}

void PlayStateWidget::setPicture(QNetworkReply *reply)
{
    QByteArray picData;
    picData = reply->readAll();
    QPixmap pix;
    pix.loadFromData(picData);
    pix = pix.scaled(50,50);
    ui->picture->setPixmap(pix);
}

void PlayStateWidget::on_downloadBtn_clicked()
{
    downloader.downloadMusic(curMusic);
    ui->downloadBtn->setIcon(QIcon(":/image/resource/complete.png"));
}

void PlayStateWidget::on_likeBtn_clicked()
{
    if(curMusic->like){
        ui->likeBtn->setIcon(QIcon(":/image/resource/like_no.png"));
        curMusic->like = false;
        emit dislikeMusic(curMusic);
    }
    else{
        ui->likeBtn->setIcon(QIcon(":/image/resource/like_yes.png"));
        curMusic->like = true;
        emit likeMusic(curMusic);
    }
}
