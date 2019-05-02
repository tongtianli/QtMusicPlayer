#include "findmusicwidget.h"
#include "ui_findmusicwidget.h"

FindMusicWidget::FindMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindMusicWidget)
{
    ui->setupUi(this);

    connectSlots();
}

FindMusicWidget::~FindMusicWidget()
{
    delete ui;
}

void FindMusicWidget::on_searchBox_returnPressed()
{
    searchContect = ui->searchBox->text();
    sendRequest();
}

void FindMusicWidget::handleNetworkData(QNetworkReply *networkReply)
{
    if (networkReply->error() != QNetworkReply::NoError) {
        qDebug()<<"NetworkReply error!";
        return;
    }
    QByteArray response(networkReply->readAll());
    QJsonParseError jsonError;
    QJsonDocument parseDocument = QJsonDocument::fromJson(response,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError){
        qDebug()<<"Json parse error!";
    }
    QJsonObject object = parseDocument.object();
    QJsonValueRef data = object["data"];
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        musicCache.clear();
        while(ui->musicTable->rowCount())
            ui->musicTable->removeRow(0);
        QJsonArray songs = data.toArray();
        for(int i=0;i<songs.size();i++){
            QJsonObject music = songs.at(i).toObject();
            Music *newMusic = new Music();
            newMusic->ID = music["id"].toInt();
            newMusic->name = music["name"].toString();
            newMusic->singer = music["singer"].toString();
            newMusic->pic = music["pic"].toString();
            newMusic->url = music["url"].toString();
            newMusic->lrc = music["lrc"].toString();
            int time = music["time"].toInt();
            newMusic->duration = QString("%1:%2").arg(time/60,2,10,QLatin1Char('0')).arg(time%60,2,10,QLatin1Char('0'));         musicCache.append(newMusic);
            ui->listTable->insertRow(i);
            ui->listTable->setItem(i,0,new QTableWidgetItem(newMusic->name));
            ui->listTable->setItem(i,1,new QTableWidgetItem(QString::number(newMusic->count)));
            ui->listTable->setItem(i,2,new QTableWidgetItem(newMusic->creator.nickname));
            ui->listTable->setItem(i,3,new QTableWidgetItem(QString::number(newMusic->playCount)));
            ui->listTable->setItem(i,4,new QTableWidgetItem(QString::number(newMusic->bookCount)));
        }
        break;
    case 1:
        musicCache.clear();
        while(ui->musicTable->rowCount())
            ui->musicTable->removeRow(0);
        QJsonArray songs = data.toArray();
        for(int i=0;i<songs.size();i++){
            QJsonObject playlist = songs.at(i).toObject();
            Playlist *newlist = new Playlist();
            newlist->id = playlist["id"].toInt();
            newlist->name = playlist["name"].toString();
            newlist->count = playlist["trackCount"].toInt();
            newlist->coverImgUrl = QUrl(playlist["coverImgUrl"].toString());
            newlist->creator.nickname = playlist["creator"].toObject()["nickname"].toString();
            newlist->playCount = playlist["playCount"].toInt();
            newlist->bookCount = playlist["bookCount"].toInt();
            playlistCache.append(newlist);
            ui->listTable->insertRow(i);
            ui->listTable->setItem(i,0,new QTableWidgetItem(newlist->name));
            ui->listTable->setItem(i,1,new QTableWidgetItem(QString::number(newlist->count)));
            ui->listTable->setItem(i,2,new QTableWidgetItem(newlist->creator.nickname));
            ui->listTable->setItem(i,3,new QTableWidgetItem(QString::number(newlist->playCount)));
            ui->listTable->setItem(i,4,new QTableWidgetItem(QString::number(newlist->bookCount)));
        }
        break;
    case 2:
        playlistCache.clear();
        while(ui->listTable->rowCount())
            ui->listTable->removeRow(0);
        QJsonArray playlists = data.toObject()["playlists"].toArray();
        for(int i=0;i<playlists.size();i++){
            QJsonObject playlist = playlists.at(i).toObject();
            Playlist *newlist = new Playlist();
            newlist->id = playlist["id"].toInt();
            newlist->name = playlist["name"].toString();
            newlist->count = playlist["trackCount"].toInt();
            newlist->coverImgUrl = QUrl(playlist["coverImgUrl"].toString());
            newlist->creator.nickname = playlist["creator"].toObject()["nickname"].toString();
            newlist->playCount = playlist["playCount"].toInt();
            newlist->bookCount = playlist["bookCount"].toInt();
            playlistCache.append(newlist);
            ui->listTable->insertRow(i);
            ui->listTable->setItem(i,0,new QTableWidgetItem(newlist->name));
            ui->listTable->setItem(i,1,new QTableWidgetItem(QString::number(newlist->count)));
            ui->listTable->setItem(i,2,new QTableWidgetItem(newlist->creator.nickname));
            ui->listTable->setItem(i,3,new QTableWidgetItem(QString::number(newlist->playCount)));
            ui->listTable->setItem(i,4,new QTableWidgetItem(QString::number(newlist->bookCount)));
        }
        break;
    }
}

void FindMusicWidget::connectSlots()
{
    connect(&networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(handleNetworkData(QNetworkReply*)));
}

void FindMusicWidget::sendRequest()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        searchUrl = QString("https://api.itooi.cn/music/netease/search?key=579621905&s=%1&type=song&limit=100&offset=0").arg(searchContect);
        break;
    case 1:
        searchUrl = QString("https://api.itooi.cn/music/netease/search?key=579621905&s=%1&type=album&limit=20&offset=0").arg(searchContect);
        break;
    case 2:
        searchUrl = QString("https://api.itooi.cn/music/netease/search?key=579621905&s=%1&type=list&limit=20&offset=0").arg(searchContect);
    }
    qDebug()<<"searchUrl:"<<searchUrl;
    networkManager.get(QNetworkRequest(searchUrl));
}

void FindMusicWidget::showResult()
{

}

void FindMusicWidget::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    sendRequest();
}

void FindMusicWidget::on_btnSearch_clicked()
{
    on_searchBox_returnPressed();
}
