#include "currentmusicwidget.h"
#include "ui_currentmusicwidget.h"

CurrentMusicWidget::CurrentMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurrentMusicWidget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    connect(&pic,&QNetworkAccessManager::finished,
            this,&CurrentMusicWidget::setPicture);
    connect(&lrc,&QNetworkAccessManager::finished,
            this,&CurrentMusicWidget::setLrc);
    ui->lrc->setWindowOpacity(0.5);
    hide();
}

CurrentMusicWidget::~CurrentMusicWidget()
{
    delete ui;
}

void CurrentMusicWidget::requestPicture(QUrl picUrl)
{
    if(picUrl.isValid())
        pic.get(QNetworkRequest(picUrl));
}

void CurrentMusicWidget::requestLrc(QUrl lrcUrl)
{
    if(lrcUrl.isValid())
        lrc.get(QNetworkRequest(lrcUrl));
}

void CurrentMusicWidget::setPicture(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError){
        return;
    }
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    pix.scaled(size());
    QPalette pal = this->palette();
    pal.setBrush(backgroundRole(), pix);
    setPalette(pal);
}

void CurrentMusicWidget::setLrc(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError){
        return;
    }
    ui->lrc->setText(QString(reply->readAll()));
}
