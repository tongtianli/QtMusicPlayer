#include "musictablewidget.h"

MusicTableWidget::MusicTableWidget(QWidget *parent, QString tableName) :
    QTableWidget(parent)
{
    name = tableName;
    preventChangeSignal = false;
    connect(this,&QTableWidget::itemDoubleClicked,this,&MusicTableWidget::onTableItemClicked);
    setEditTriggers(EditTrigger::NoEditTriggers);
    horizontalHeader();
}


void MusicTableWidget::setName(QString tableName)
{
    name = tableName;
}

void MusicTableWidget::insertMusic(int index, Music *music)
{
    list.insert(index,music);
//    if(music->ID != -1){
//        idset.insert(music->ID);
//    }
    this->insertRow(index);
    for(int i=0;i<this->columnCount();i++){
        QString headerText = this->horizontalHeaderItem(i)->text();
        if(headerText == "音乐标题"||headerText == "正在播放"){
            this->setItem(index,i,new QTableWidgetItem(music->name));
            continue;
        }
        if(headerText == "歌手"){
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
//    idset.remove(list.at(index)->ID);
    list.removeAt(index);
    if(!preventChangeSignal)
        emit sizeChanged(list.size());
}

void MusicTableWidget::append(Music *music)
{
    int size = list.size();
    size = size<0?0:size;
    insertMusic(size,music);
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
    reader.readNextStartElement();
    qDebug()<<reader.text();
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
}

void MusicTableWidget::save()
{
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

void MusicTableWidget::onTableItemClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    int row = this->currentRow();
    emit musicDoubleClicked(list,row);
}

