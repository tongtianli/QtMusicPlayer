#include "musiclistlistwidget.h"


MusiclistListWidget::MusiclistListWidget(QWidget *parent) : QListWidget(parent)
{
    Q_UNUSED(parent);
    setMouseTracking(true);
    buildMenu();
    question = new AddNewMusiclist(this);
    question->hide();
    connect(question,&AddNewMusiclist::addlist,this,&MusiclistListWidget::addUserMusiclist);
}

MusiclistListWidget::~MusiclistListWidget()
{
    delete question;
}

void MusiclistListWidget::buildMenu()
{
    menu = new QMenu(this);
    QAction *create = new QAction(QIcon(":/image/resource/addlist.png"),"添加",this);
    connect(create,&QAction::triggered,this,&MusiclistListWidget::onActionAddTriggered);
    menu->addAction(create);
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
            name_widgetHash.insert("发现音乐",findMusicWidget);
            continue;
        }
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
            favoriteMusicWidget->setName("我喜欢的音乐");
            connect(favoriteMusicWidget->table,&MusicTableWidget::musicDoubleClicked,playlistWidget,&PlayListWidget::changeListAndPlay);

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
    userMusiclist->setName(listname);
    addItem(new QListWidgetItem(QIcon(":/image/resource/music.png"),listname));
    name_widgetHash.insert(listname,userMusiclist);
    QDate date = QDate::currentDate();
    QString dateText = QString::asprintf("%d-%d-%d",date.year(),date.month(),date.day());
    userMusiclist->setInitDate(dateText);
}

void MusiclistListWidget::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if(event->button()==Qt::RightButton){
        menu->popup(QCursor::pos());
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
    QPoint position = QWidget::mapFromGlobal(QCursor::pos());
    question->setGeometry(position.x(),position.y(),150,150);
    question->show();
}


void MusiclistListWidget::saveUserMusiclist()
{
//    foreach(QString name, name_widgetHash.keys()){
//        if(defaultListnames.contains(name))
//            continue;

//    }
}
