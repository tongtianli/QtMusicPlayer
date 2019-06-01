#include "musiclistlistwidget.h"


MusiclistListWidget::MusiclistListWidget(QWidget *parent) : QListWidget(parent)
{
    Q_UNUSED(parent);
    setMouseTracking(true);
    buildMenu();
    addNewMusiclist = new AddNewMusiclist(this);
    connect(addNewMusiclist,&AddNewMusiclist::addlist,this,&MusiclistListWidget::addUserMusiclist);
}

MusiclistListWidget::~MusiclistListWidget()
{
    delete addNewMusiclist;
    delete itemMenu_user;
    delete menu;
    delete itemMenu_default;
}

void MusiclistListWidget::buildMenu()
{
    menu = new QMenu(this);
    itemMenu_user = new QMenu(this);
    itemMenu_default = new QMenu(this);
    QAction *create = new QAction(QIcon(":/image/resource/addlist.png"),"添加",this);
    connect(create,&QAction::triggered,this,&MusiclistListWidget::onActionAddTriggered);
    menu->addAction(create);
    QAction *play = new QAction(QIcon(":/image/resource/menu_play.png"),"播放",this);
    connect(play,&QAction::triggered,this,&MusiclistListWidget::onActionPlayTriggered);
    itemMenu_user->addAction(play);
    itemMenu_default->addAction(play);
    QAction *playLater = new QAction(QIcon(":/image/resource/playLater.png"),"下一首播放",this);
    connect(playLater,&QAction::triggered,this,&MusiclistListWidget::onActionPlayLaterTriggered);
    itemMenu_user->addAction(playLater);
    itemMenu_default->addAction(playLater);

    itemMenu_user->addSeparator();
    itemMenu_default->addSeparator();

    QAction *edit = new QAction(QIcon(":/image/resource/edit.png"),"重命名",this);
    connect(edit,&QAction::triggered,this,&MusiclistListWidget::onActionEditTriggered);
    itemMenu_user->addAction(edit);

    QAction *download = new QAction(QIcon(":/image/resource/downloadAll.png"),"下载全部",this);
    connect(download,&QAction::triggered,this,&MusiclistListWidget::onActionDownloadTriggered);
    itemMenu_user->addAction(download);

    QAction *deletelist = new QAction(QIcon(":/image/resource/delete.png"),"删除",this);
    connect(deletelist,&QAction::triggered,this,&MusiclistListWidget::onActionDeletelistTriggered);
    itemMenu_user->addAction(deletelist);

}


void MusiclistListWidget::setup(QWidget *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget)
{
    this->parent = parent;
    this->scrollArea = scrollArea;
    this->playlistWidget = playlistWidget;

}


void MusiclistListWidget::initialDefaultWidgets()
{
    foreach(QString name, defaultListnames){
        if(name=="发现音乐"){
            FindMusicWidget *findMusicWidget = new FindMusicWidget(parent);
            findMusicWidget->hide();
            connect(findMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
            connect(this,&MusiclistListWidget::listChanged,findMusicWidget->table,&MusicTableWidget::buildBookMenu);
            name_widgetHash.insert("发现音乐",findMusicWidget);
            continue;
        }
        else if(name=="本地音乐"){
            LocalMusicWidget *localMusicWidget = new LocalMusicWidget(parent);
            localMusicWidget->load();
            localMusicWidget->hide();
            connect(localMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
            connect(localMusicWidget->table,&MusicTableWidget::playThisListLater,playlistWidget,&PlayListWidget::playListLater);
            connect(this,&MusiclistListWidget::listChanged,localMusicWidget->table,&MusicTableWidget::buildBookMenu);
            name_widgetHash.insert("本地音乐",localMusicWidget);
            continue;
        }
        else if(name=="我喜欢的音乐"){
            UserMusicWidget *favoriteMusicWidget = new UserMusicWidget(parent);
            favoriteMusicWidget->hide();
            favoriteMusicWidget->setName("我喜欢的音乐");
            connect(favoriteMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
            connect(favoriteMusicWidget->table,&MusicTableWidget::playThisListLater,playlistWidget,&PlayListWidget::playListLater);
            connect(this,&MusiclistListWidget::listChanged,favoriteMusicWidget->table,&MusicTableWidget::buildBookMenu);
            favoriteMusicWidget->load();
            name_widgetHash.insert("我喜欢的音乐",favoriteMusicWidget);
            continue;
        }
    }
    emit listChanged(name_widgetHash);
}


void MusiclistListWidget::loadUserMusiclists()
{

}

void MusiclistListWidget::deleteList(QString name)
{
    QWidget *widget = name_widgetHash.value(name);
    int i;
    for(i=0;i<count();i++){
        if(item(i)->text()==name)
            break;
    }
    if(i!=count())
        takeItem(i);
    widget->deleteLater();
    name_widgetHash.remove(name);
}

void MusiclistListWidget::addUserMusiclist(QString listname)
{
    if(name_widgetHash.contains(listname))
        return;
    if(defaultListnames.contains(listname))
        return;
    UserMusicWidget *userMusiclist = new UserMusicWidget(parent);
    connect(userMusiclist->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);
    connect(userMusiclist->table,&MusicTableWidget::playThisListLater,playlistWidget,&PlayListWidget::playListLater);
    connect(this,&MusiclistListWidget::listChanged,userMusiclist->table,&MusicTableWidget::buildBookMenu);
    userMusiclist->setName(listname);
    addItem(new QListWidgetItem(QIcon(":/image/resource/userPlaylistIcon.png"),listname));
    name_widgetHash.insert(listname,userMusiclist);
    QDate date = QDate::currentDate();
    QString dateText = QString::asprintf("%d-%d-%d",date.year(),date.month(),date.day());
    userMusiclist->setInitDate(dateText);
    emit listChanged(name_widgetHash);
}

void MusiclistListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if(event->button()==Qt::RightButton){
        QPoint position = QWidget::mapFromGlobal(QCursor::pos());
        if(itemAt(position)){
            curListName = itemAt(position)->text();
            if(defaultListnames.contains(curListName)){
                //鼠标放在默认歌单上
                if(curListName!="发现音乐")
                    itemMenu_default->popup(QCursor::pos());
            }
            else{
                //鼠标放在用户歌单上
                qDebug()<<curListName;
                itemMenu_user->popup(QCursor::pos());
            }
        }
        else{
            //鼠标放在界面中，而非任一歌单上
            menu->popup(QCursor::pos());
        }
    }
}

void MusiclistListWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    emit sizeChanged();
}

void MusiclistListWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QString curName = item(current.row())->text();
    scrollArea->takeWidget();
    scrollArea->setWidget(name_widgetHash.value(curName));
    QListWidget::currentChanged(current,previous);
}

void MusiclistListWidget::onActionAddTriggered(bool checked)
{
    Q_UNUSED(checked);
    QPoint position = QCursor::pos();
    addNewMusiclist->setGeometry(position.x(),position.y(),150,150);
    addNewMusiclist->show();
}

void MusiclistListWidget::onActionPlayTriggered(bool checked)
{
    Q_UNUSED(checked);
    QWidget *curWidget = name_widgetHash.value(curListName);
    if(curListName=="本地音乐"){
        LocalMusicWidget *local = qobject_cast<LocalMusicWidget*>(curWidget);
        local->table->play();
    }
    else{
        UserMusicWidget *userOrFavourite = qobject_cast<UserMusicWidget*>(curWidget);
        userOrFavourite->table->play();
    }

}

void MusiclistListWidget::onActionPlayLaterTriggered(bool checked)
{
    Q_UNUSED(checked);
    QWidget *curWidget = name_widgetHash.value(curListName);
    if(curListName=="本地音乐"){
        LocalMusicWidget *local = qobject_cast<LocalMusicWidget*>(curWidget);
        local->table->playLater();
    }
    else{
        UserMusicWidget *userOrFavourite = qobject_cast<UserMusicWidget*>(curWidget);
        userOrFavourite->table->playLater();
    }
}

void MusiclistListWidget::onActionEditTriggered(bool checked)
{
    Q_UNUSED(checked);
    QWidget *curWidget = name_widgetHash.value(curListName);
    UserMusicWidget *widget = qobject_cast<UserMusicWidget*>(curWidget);
    bool *ok = new bool();
    QString newName = QInputDialog::getText(this,"重命名歌单","新歌单名",QLineEdit::Normal,widget->name(),ok);
    if(ok){
        int i;
        for(i=0;i<count();i++)
            if(item(i)->text()==curListName)
                break;
        if(i!=count())
            item(i)->setText(newName);
        widget->setName(newName);
        emit listChanged(name_widgetHash);
    }
}

void MusiclistListWidget::onActionDownloadTriggered(bool checked)
{
    Q_UNUSED(checked);
    QWidget *curWidget = name_widgetHash.value(curListName);
    UserMusicWidget *widget = qobject_cast<UserMusicWidget*>(curWidget);
    widget->table->downloadAllMusic();
}

void MusiclistListWidget::onActionDeletelistTriggered(bool checked)
{
    Q_UNUSED(checked);
    deleteList(curListName);
    emit listChanged(name_widgetHash);
}

void MusiclistListWidget::saveUserMusiclist()
{
//    foreach(QString name, name_widgetHash.keys()){
//        if(defaultListnames.contains(name))
//            continue;

//    }
}
