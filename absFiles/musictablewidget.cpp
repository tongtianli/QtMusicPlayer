#include "musictablewidget.h"

#include "usermusicwidget.h"

MusicTableWidget::MusicTableWidget(QWidget *parent, QString tableName) :
    QTableWidget(parent)
{
    name = tableName;
    preventChangeSignal = false;
    connect(this,&QTableWidget::itemDoubleClicked,this,&MusicTableWidget::onTableItemDoubleClicked);
    setEditTriggers(EditTrigger::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    horizontalHeader()->setStretchLastSection(true);
    menu = new QMenu(this);
    actPlay = new QAction(QIcon(":/image/resource/menu_play.png"),"播放",this);
    connect(actPlay,&QAction::triggered,this,&MusicTableWidget::onActionPlayTriggered);
    actPlayLater = new QAction(QIcon(":/image/resource/playLater.png"),"下一首播放",this);
    connect(actPlayLater,&QAction::triggered,this,&MusicTableWidget::onActionPlayLaterTriggered);
    actDownload = new QAction(QIcon(":/image/resource/downloadAll.png"),"下载",this);
    connect(actDownload,&QAction::triggered,this,&MusicTableWidget::onActionDownloadTriggered);
    actRemove = new QAction(QIcon(":/image/resource/delete.png"),"删除",this);
    connect(actRemove,&QAction::triggered,this,&MusicTableWidget::onActionRemoveTriggered);
    menu->addAction(actPlay);
    menu->addAction(actPlayLater);
    menu->addSeparator();
    bookMenu = menu->addMenu(QIcon(":/image/resource/book.png"),"添加到");
    menu->addAction(actDownload);
    menu->addAction(actRemove);
    connect(bookMenu,&QMenu::triggered,this,&MusicTableWidget::addSelectionsToMusiclist);
}

MusicTableWidget::~MusicTableWidget()
{
    save();
    delete actPlay;
    delete actPlayLater;
    delete actDownload;
    delete actRemove;

    foreach(QAction *act,bookMenu->actions()){
        bookMenu->removeAction(act);
        delete act;
    }
    delete bookMenu;
    delete menu;
}


void MusicTableWidget::setName(QString tableName)
{
    name = tableName;
}

void MusicTableWidget::insertMusic(int index, Music *music)
{
    if(musicSet.contains(music->name+music->singer)){
        for(int i = 0;i<list.size();i++){
            if(list.at(i)->name==music->name&&list.at(i)->singer==music->singer){
                this->remove(i);
                index = 0;
                break;
            }
        }
    }
    else{
        musicSet.insert(music->name+music->singer);
    }
    list.insert(index,music);
    this->insertRow(index);
    for(int i=0;i<this->columnCount();i++){
        QString headerText = this->horizontalHeaderItem(i)->text();
        if(headerText == "音乐标题"||headerText == "正在播放"){
            this->setItem(index,i,new QTableWidgetItem(music->name));
            continue;
        }
        if(headerText == "歌手"||headerText == "作者"){
            this->setItem(index,i,new QTableWidgetItem(music->singer));
            continue;
        }
        if(headerText == "时长"){
            this->setItem(index,i,new QTableWidgetItem(music->duration));
            continue;
        }
        if(headerText == "大小"){
            this->setItem(index,i,new QTableWidgetItem(music->size));
            continue;
        }
        if(headerText == "播放时间"){
            this->setItem(index,i,new QTableWidgetItem(QTime::currentTime().toString()));
            continue;
        }
    }
    if(!preventChangeSignal)
        emit sizeChanged(list.size());
}

void MusicTableWidget::setMusiclist(QList<Music *> list)
{
    preventChangeSignal = true;
    this->list=list;
    foreach(Music* music, list){
        append(music);
    }
    emit sizeChanged(list.size());
    preventChangeSignal = false;
}

void MusicTableWidget::clearAll()
{
    preventChangeSignal = true;
    while(rowCount()>0)
        remove(0);
    emit sizeChanged(list.size());
    preventChangeSignal = false;
}

void MusicTableWidget::remove(int index)
{
    removeRow(index);
    musicSet.remove(list.at(index)->name+list.at(index)->singer);
    list.removeAt(index);
    if(!preventChangeSignal)
        emit sizeChanged(list.size());
}

void MusicTableWidget::remove(Music *music)
{
    for(int i = 0;i<list.size();i++){
        if(list.at(i)->name==music->name&&list.at(i)->singer==music->singer){
            this->remove(i);
            return;
        }
    }
}

void MusicTableWidget::append(Music *music)
{
    int size = list.size();
    size = size<0?0:size;
    insertMusic(size,music);
}

void MusicTableWidget::play()
{
    emit musicDoubleClicked(list,0);
}

void MusicTableWidget::playLater()
{
    emit playThisListLater(list);
}

void MusicTableWidget::buildBookMenu(QHash<QString, QWidget *> name_widgetHash)
{
    foreach(QAction* act, bookMenu->actions()){
        bookMenu->removeAction(act);
        delete act;
    }
    foreach(QString name, name_widgetHash.keys()){
        if(defaultListnames.contains(name)&&name!="我喜欢的音乐")
            continue;
        QAction *act = new QAction(QIcon(":/image/resource/userPlaylistIcon.png"),name,this);
        bookMenu->addAction(act);
    }
    this->name_widgetHash = name_widgetHash;
}

Music *MusicTableWidget::get(int index)
{
    return list[index];
}

QList<Music *> MusicTableWidget::getAll()
{
    return list;
}

void MusicTableWidget::load()
{
    QString fileDir = QDir::currentPath() +"/data/"+ name +".xml";
    QFile file(fileDir);
    if(not file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"No cache file:"<<fileDir;
        return;
    }
    QXmlStreamReader reader(&file);
    bool success = reader.readNextStartElement();
    if(!success)
        return;
    if(reader.isStartElement()&&reader.name()!="musiclist")
        return;
    name = reader.attributes().at(0).value().toString();
    reader.readNextStartElement();
    Music *music = nullptr;
    while(!reader.atEnd()){
        if(reader.isStartElement()&&reader.name()=="music"){
            if(music!=nullptr)
                append(music);
            music = new Music();
            reader.readNext();
        }
        if(reader.name()=="id")
            music->ID = reader.readElementText().toInt();
        else if(reader.name()=="name")
            music->name = reader.readElementText();
        else if(reader.name()=="singer")
            music->singer = reader.readElementText();
        else if(reader.name()=="duration")
            music->duration = reader.readElementText();
        else if(reader.name()=="pic")
            music->pic = reader.readElementText();
        else if(reader.name()=="lrc")
            music->lrc = reader.readElementText();
        else if(reader.name()=="url")
            music->url = reader.readElementText();
        else if(reader.name()=="local")
            music->local = reader.readElementText()=="true"?true:false;
        else if(reader.name()=="size")
            music->size = reader.readElementText();
        reader.readNext();
    }
    if(music!=nullptr)
        append(music);

}

void MusicTableWidget::downloadAllMusic()
{
    MusicDownloader downloader;
    for(int i=0;i<list.count();i++)
        downloader.downloadMusic(list.at(i));
}

void MusicTableWidget::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    if(event->button()==Qt::RightButton){
        QPoint position = event->pos();
        if(itemAt(position)){
            menu->popup(QCursor::pos());
        }
    }
}
void MusicTableWidget::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTableWidget::selectionChanged(selected,deselected);
    QSet<int> selectedIndexes;
    for(int i=0;i<selectedItems().size();i++){
        int row = selectedItems().at(i)->row();
        selectedIndexes.insert(row);
    }
    sortedSelectionIndexes = selectedIndexes.toList();
    std::sort(sortedSelectionIndexes.begin(),sortedSelectionIndexes.end());
}

void MusicTableWidget::save()
{
    QString path = QDir::currentPath() + "/data";
    QDir pathDir(path);
    if(!pathDir.exists())
        pathDir.mkdir(path);
    QString fileDir = QDir::currentPath() +"/data/"+ name +".xml";
    QFile file(fileDir);
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<fileDir<<endl;
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("musiclist");
    writer.writeAttribute("listname",name);
    for(int i=0;i<list.count();i++){
        Music *music = list[i];
        writer.writeStartElement("music");
        writer.writeTextElement("id",QString::number(music->ID,10));
        writer.writeTextElement("name",music->name);
        writer.writeTextElement("singer",music->singer);
        writer.writeTextElement("duration",music->duration);
        writer.writeTextElement("pic",music->pic.toString());
        writer.writeTextElement("lrc",music->lrc.toString());
        writer.writeTextElement("url",music->url.toString());
        writer.writeTextElement("local",music->local?"true":"false");
        writer.writeTextElement("size",music->size);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void MusicTableWidget::onTableItemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    int row = this->currentRow();
    emit musicDoubleClicked(list,row);
}

void MusicTableWidget::addSelectionsToMusiclist(QAction *action)
{
    QString listname = action->text();
    UserMusicWidget *widget = qobject_cast<UserMusicWidget*>(name_widgetHash.value(listname));
    for(int i=sortedSelectionIndexes.size();i>=0;i--)
        widget->prepend(list[i]);
}

void MusicTableWidget::onActionRemoveTriggered()
{
    for(int i=sortedSelectionIndexes.size()-1;i>=0;i--){
        remove(sortedSelectionIndexes[i]);
    }
}

void MusicTableWidget::onActionPlayTriggered()
{
    QList<Music*> playlist;
    for(int i=0;i<sortedSelectionIndexes.size();i++)
        playlist.append(list.at(sortedSelectionIndexes[i]));
    emit musicDoubleClicked(playlist,0);
}

void MusicTableWidget::onActionPlayLaterTriggered()
{
    QList<Music*> playlist;
    for(int i=0;i<sortedSelectionIndexes.size();i++)
        playlist.append(list.at(sortedSelectionIndexes[i]));
    emit playThisListLater(playlist);
}

void MusicTableWidget::onActionDownloadTriggered()
{
    MusicDownloader downloader;
    for(int i=0;i<sortedSelectionIndexes.count();i++)
        downloader.downloadMusic(list.at(sortedSelectionIndexes[i]));
}

void MusicTableWidget::saveCurrentSelection()
{
    this->bookSelection = bookMenu->actionAt(QWidget::mapFromGlobal(QCursor::pos()))->text();
}

