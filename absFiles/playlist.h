#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "music.h"

typedef struct User{
    QString nickname;
}User;

typedef struct Playlist{
    qint64 id;
    QString name;
    int count;
    QUrl coverImgUrl;
    User creator;
    int playCount;
    int bookCount;
    QString description;
}Playlist;

typedef struct Artist{
    QString name;
    QUrl picUrl;
    QString alias;      //别名
}Artist;

typedef struct Album{
    QString name;
    qint64 id;
    int size;
    QUrl picUrl;
    Artist artist;
    QString alias;      //别名
}Album;

#endif // PLAYLIST_H
