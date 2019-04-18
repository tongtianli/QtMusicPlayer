#include "musiclistlistwidget.h"

MusiclistListWidget::MusiclistListWidget()
{

}

MusiclistListWidget::MusiclistListWidget(QObject *parent) : QListWidget()
{
    Q_UNUSED(parent);
}

void MusiclistListWidget::setup(QMainWindow *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget)
{
    this->parent = parent;
    this->scrollArea = scrollArea;
    this->playlistWidget = playlistWidget;
}


void MusiclistListWidget::initialDefaultWidgets()
{
    foreach(QString name, defaultListnames){
        //if(name=="发现音乐");
        if(name=="本地音乐"){
            LocalMusicWidget *localMusicWidget = new LocalMusicWidget(parent);
            localMusicWidget->load();
            localMusicWidget->hide();
            connect(localMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
            name_widgetHash.insert("本地音乐",localMusicWidget);
            continue;
        }
        if(name=="我喜欢的音乐"){
            UserMusicWidget *favoriteMusicWidget = new UserMusicWidget(parent);
            favoriteMusicWidget->hide();
            favoriteMusicWidget->setTableName("我喜欢的音乐");
            favoriteMusicWidget->load();
            name_widgetHash.insert("我喜欢的音乐",favoriteMusicWidget);
            continue;
        }
    }



}

void MusiclistListWidget::loadUserMusiclists()
{

}

void MusiclistListWidget::addUserMusiclist(QString listname)
{
    UserMusicWidget *userMusiclist = new UserMusicWidget(parent);
    userMusiclist->setTableName(listname);
}

void MusiclistListWidget::saveUserMusiclist()
{
//    foreach(QString name, name_widgetHash.keys()){
//        if(defaultListnames.contains(name))
//            continue;

//    }
}
