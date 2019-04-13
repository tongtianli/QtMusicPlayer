#include "musicpool.h"

//music创建与销毁

MusicPool::MusicPool(QString name)
{
    this->name = name;
}

MusicPool::~MusicPool()
{
    foreach(Music* music,list.values()){
        delete music;
    }
}

void MusicPool::addMusic(Music *music)
{
    while(list.contains(music->ID)){
        Music *m = list.value(music->ID);
        if(m->name==music->name&&m->url==music->url) return;
        else music->ID++;
    }
    list.insert(music->ID,music);
}

Music *MusicPool::getMusicByID(int ID)
{
    return list.value(ID);
}

int MusicPool::getID(Music *music)
{
    return list.key(music);
}

int MusicPool::size()
{
    return list.size();
}

inline bool notEmpty(QString s){

    return not (s.isNull()||s.isEmpty());
}

void MusicPool::save()
{
    QDir dir = QDir::currentPath()+"/data/"+name+".xml";
    QFile file(dir.path());
    QHashIterator<int,Music*> iter(list);
    iter.toFront();
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<dir<<endl;
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Default_Music_pool");
    for(int i=0;i<list.size();i++){
        iter.next();
        Music* music = iter.value();
        writer.writeStartElement("song");
        writer.writeTextElement("iterNo.",QString::number(i));
        writer.writeTextElement("mediaID",QString::number(music->ID));
        if(notEmpty(music->name))
            writer.writeTextElement("name",music->name);
        if(notEmpty(music->singer))
            writer.writeTextElement("author",music->singer);
        if(notEmpty(music->size))
            writer.writeTextElement("size",music->size);
        if(notEmpty(music->duration))
            writer.writeTextElement("duration",music->duration);
        writer.writeTextElement("path",music->url.path());
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void MusicPool::load()
{
    QDir dir = QDir::currentPath()+"/data/"+name+".xml";
    QFile file(dir.path());
    if(not file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"can not open"<<dir<<endl;
        return;
    }
    QXmlStreamReader reader(&file);
    Music *music = new Music();
    while(!reader.atEnd()){
        while(not reader.isEndElement()){
            if(reader.name()=="mediaID")
                music->ID = reader.readElementText().toInt();
            else if(reader.name()=="name")
                music->name = reader.readElementText();
            else if(reader.name()=="duration")
                music->duration = reader.readElementText();
            else if(reader.name()=="size")
                music->size = reader.readElementText();
            else if(reader.name()=="path"){
                music->url = reader.readElementText();
            }
            reader.readNext();
        }
        if(reader.isEndElement()&&reader.name()=="song"){
            list.insert(music->ID,music);
            music = new Music();
        }
        reader.readNext();
    }
    if(music->url.isEmpty()) delete music;
    file.close();
}

