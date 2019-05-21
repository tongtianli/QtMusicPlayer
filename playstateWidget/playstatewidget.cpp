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
        qDebug()<<"online file";
        manager.get(QNetworkRequest(picUrl));
    }
    else {
        qDebug()<<"local file";
        //defaultPix = QPixmap(":/image/resource/localMusicPic.png").scaled(50,50);
        ui->picture->setPixmap(defaultPix);
    }
    ui->author->setText(music->singer);
    ui->songName->setText(music->name);
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
