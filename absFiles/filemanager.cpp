#include "filemanager.h"

FileManager::FileManager(QString filename)
{
    this->defaultDir = QDir::currentPath() +"/"+ filename +"/xml";
}

QMediaPlaylist* FileManager::loadPlaylist(QObject *parent, QTableWidget *musicTable)
{
    QFile file(defaultDir);
    QMediaPlaylist *playlist = new QMediaPlaylist(parent);
    if(not file.open(QFile::ReadOnly | QFile::Text))
    {
        return playlist;
    }
    QXmlStreamReader reader(&file);
    while(!reader.atEnd()){
        qDebug()<<reader.name()<<endl;
        if(reader.isStartElement()){
            if(reader.name()=="path"){
                QString musicDir = reader.readElementText();
                playlist->addMedia(QMediaContent(musicDir));
                qDebug()<<musicDir<<endl;
            }
            else if(reader.name()=="name"){
                musicTable->insertRow(musicTable->rowCount());
                QString musicName = reader.readElementText();
                QTableWidgetItem *item = new QTableWidgetItem(musicName);
                musicTable->setItem(musicTable->rowCount()-1,0,item);
                qDebug()<<musicName<<endl;
            }
            else if(reader.name()=="duration"){
                QString duration = reader.readElementText();
                QTableWidgetItem *item = new QTableWidgetItem(duration);
                musicTable->setItem(musicTable->rowCount()-1,1,item);
                qDebug()<<duration<<endl;
            }
            else if(reader.name()=="size"){
                QString musicSize = reader.readElementText();
                QTableWidgetItem *item = new QTableWidgetItem(musicSize);
                musicTable->setItem(musicTable->rowCount()-1,2,item);
                qDebug()<<musicSize<<endl;
            }
        }
        reader.readNext();
    }


    return playlist;
}

bool FileManager::savePlaylist(QTableWidget *musicTable, QMediaPlaylist *playlist)
{
    QFile file(defaultDir);
    if(not file.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        qDebug()<<"can not open"<<defaultDir<<endl;
        return false;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("Default_Music_list");
    for(int i=0;i<musicTable->rowCount();i++){
        QMediaContent item = playlist->media(i);
        QUrl mediaPath = item.canonicalUrl();
        QString musicName = musicTable->takeItem(i,0)->text();
        QTableWidgetItem *duration = musicTable->takeItem(i,1);
        QString size = musicTable->takeItem(i,2)->text();

        writer.writeStartElement("song");
        writer.writeAttribute("ID",QString::number(i,10));
        writer.writeTextElement("path",mediaPath.toString());
        writer.writeTextElement("name",musicName);
        writer.writeTextElement("size",size);
        writer.writeTextElement("duration",duration->text());
        //writer.writeTextElement("genre"))
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    return true;
}


