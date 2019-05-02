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

#endif // PLAYLIST_H
