#include "usermusicwidget.h"
#include "ui_usermusicwidget.h"

UserMusicWidget::UserMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMusicWidget)
{
    ui->setupUi(this);
    this->hide();
    table = ui->tableWidget;
    table->setColumnWidth(0,300);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,100);

    defaultPix = QPixmap(":/image/resource/default_list.png");
    ui->picture->setPixmap(defaultPix);

    connect(table,&MusicTableWidget::sizeChanged,
            this,&UserMusicWidget::onTableSizeChanged);
    connect(&manager,&QNetworkAccessManager::finished,
            this,&UserMusicWidget::setPicture);
}
UserMusicWidget::~UserMusicWidget()
{
    delete ui;
}

void UserMusicWidget::prepend(Music *music)
{
    table->insertMusic(0,music);
    if(!music->local)
        manager.get(QNetworkRequest(music->pic));
    emit addMusic(music);
}

void UserMusicWidget::remove(Music *music)
{
    table->remove(music);
}

void UserMusicWidget::setName(QString name)
{
    table->setName(name);
    listname = name;
    ui->nameLbl->setText(name);
}

QString UserMusicWidget::name()
{
    return  listname;
}

void UserMusicWidget::setInitDate(QString dateText)
{
    ui->createTime->setText(dateText+"创建");
}

void UserMusicWidget::setList(QHash<int, Music *> musicpool, QList<int> idlist)
{
    table->clearAll();
    for(int i=0;i<idlist.size();i++){
        Music *music = musicpool.value(idlist[i]);
        table->append(music);
    }
}

void UserMusicWidget::requestPicture(QUrl url)
{
    if(url.isValid())
        manager.get(QNetworkRequest(url));
}


void UserMusicWidget::on_playAllBtn_clicked()
{
    table->play();
}

void UserMusicWidget::onTableSizeChanged(int size)
{
    ui->numLbl->setText(QString::number(size));
}

void UserMusicWidget::setPicture(QNetworkReply *reply)
{
    if(reply->error()!=QNetworkReply::NoError){
        return;
    }
    QPixmap pix;
    pix.loadFromData(reply->readAll());
    pix = pix.scaled(150,150);
    ui->picture->setPixmap(pix);
}

void UserMusicWidget::on_downloadAllBtn_clicked(){
    table->downloadAllMusic();
}


