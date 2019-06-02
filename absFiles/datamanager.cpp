#include "datamanager.h"

QHash<int, Music*> DataManager::loadMusicPool()
{
    QHash<int, Music*> hash;
    QString fileDir = QDir::currentPath() +"/data/musicpool.xml";
    QFile file(fileDir);
    if(not file.open(QFile::ReadOnly | QFile::Text))
        return hash;
    QXmlStreamReader reader(&file);
    if(!reader.readNextStartElement())
        return hash;
    Music *music = nullptr;
    while(!reader.atEnd()){
        if(reader.isStartElement()&&reader.name()=="music")
            music = new Music();
        if(reader.name()=="id")
            music->ID = reader.readElementText().toInt();
        else if(reader.name()=="quote")
            music->quote = reader.readElementText().toInt();
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
        if(reader.isEndElement()&&reader.name()=="music")
            if(music!=nullptr)
                hash.insert(music->ID,music);
        reader.readNext();
    }
    return hash;
}

QList<QString> DataManager::loadlists()
{
    QList<QString> list;
    QString fileDir = QDir::currentPath() +"/data/lists.xml";
    QFile file(fileDir);
    if(not file.open(QFile::ReadOnly | QFile::Text))
        return list;
    QXmlStreamReader reader(&file);
    if(!reader.readNext())
        return list;
    while(!reader.atEnd()){
        if(reader.name()=="listname")
            list.append(reader.readElementText());
        reader.readNext();
    }
    return list;
}

QList<int> DataManager::loadMusiclist(QString listname)
{
    QList<int> idlist;
    QString fileDir = QDir::currentPath() +"/data/"+listname+".xml";
    QFile file(fileDir);
    if(not file.open(QFile::ReadOnly | QFile::Text))
        return idlist;
    QXmlStreamReader reader(&file);
    if(!reader.readNextStartElement())
        return idlist;
    while(!reader.atEnd()){
        if(reader.name()=="id")
            idlist.append(reader.readElementText().toInt());
        reader.readNext();
    }
    return idlist;
}

void DataManager::loadLocalMusiclist(LocalMusicWidget *widget)
{
    QString fileDir = QDir::currentPath() +"/data/本地音乐.xml";
    QFile file(fileDir);
    if(not file.open(QFile::ReadOnly | QFile::Text))
        return;
    QXmlStreamReader reader(&file);
    if(!reader.readNextStartElement())
        return;
    Music *music = nullptr;
    while(!reader.atEnd()){
        if(reader.isStartElement()&&reader.name()=="music"){
            if(music!=nullptr)
                widget->table->append(music);
            music = new Music();
            reader.readNext();
        }
        if(reader.name()=="id")
            music->ID = reader.readElementText().toInt();
        else if(reader.name()=="quote")
            music->quote = reader.readElementText().toInt();
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
        widget->table->append(music);
}

void DataManager::saveMusiclist(QString name, UserMusicWidget *widget)
{
    QList<Music*> list = widget->table->getAll();
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
    writer.writeAttribute("name",name);
    for(int i=0;i<list.count();i++){
        writer.writeTextElement("id",QString::number(list[i]->ID,10));
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void DataManager::saveMusiclist(QString name, MusicTableWidget *widget)
{
    QList<Music*> list = widget->getAll();
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
    writer.writeAttribute("name",name);
    for(int i=0;i<list.count();i++){
        writer.writeTextElement("id",QString::number(list[i]->ID,10));
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void DataManager::saveLocalMusiclist(LocalMusicWidget *widget)
{
    QList<Music*> list = widget->table->getAll();
    QString path = QDir::currentPath() + "/data";
    QDir pathDir(path);
    if(!pathDir.exists())
        pathDir.mkdir(path);
    QString fileDir = QDir::currentPath() +"/data/本地音乐.xml";
    QFile file(fileDir);
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<fileDir<<endl;
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("musiclist");
    writer.writeAttribute("name","本地音乐");
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
        writer.writeTextElement("quote",QString::number(music->quote));
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void DataManager::savelists(QList<QString> lists)
{
    QString path = QDir::currentPath() + "/data";
    QDir pathDir(path);
    if(!pathDir.exists())
        pathDir.mkdir(path);
    QString fileDir = QDir::currentPath() +"/data/lists.xml";
    QFile file(fileDir);
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<fileDir<<endl;
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("lists");
    for(int i=0;i<lists.count();i++){
        writer.writeTextElement("name",lists.at(i));
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

void DataManager::saveMusicPool(QHash<int, Music *> musicPool)
{
    QList<Music*> list = musicPool.values();
    QString path = QDir::currentPath() + "/data";
    QDir pathDir(path);
    if(!pathDir.exists())
        pathDir.mkdir(path);
    QString fileDir = QDir::currentPath() +"/data/musicpool.xml";
    QFile file(fileDir);
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<fileDir<<endl;
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("musicpool");
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
        writer.writeTextElement("quote",QString::number(music->quote));
        writer.writeEndElement();
    }
    writer.writeEndDocument();
    file.close();
}

QList<Music *> DataManager::fromIDlist(QHash<int,Music*> musicpool, QList<int> idlist)
{
    QList<Music*> list;
    for(int i=0;i<idlist.size();i++){
        Music *music = musicpool.value(idlist[i]);
        list.append(music);
    }
    return list;
}

