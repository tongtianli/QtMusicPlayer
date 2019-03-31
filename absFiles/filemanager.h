#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "music.h"

class FileManager
{
public:
    FileManager();

    QMediaPlaylist* loadPlaylist(QObject *parent, QTableWidget *musicTable);
    bool savePlaylist(QTableWidget *musicTable, QMediaPlaylist *playlist);

private:
    QString defaultDir;
};

#endif // FILEMANAGER_H
