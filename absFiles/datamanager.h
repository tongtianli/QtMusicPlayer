#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "music.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    static QHash<int, Music*> loadMusicPool();
    static QList<QString> loadlists();
    static QList<int> loadMusiclist(QString listname);
    static void loadLocalMusiclist(LocalMusicWidget *widget);
    static void saveMusiclist(QString name, UserMusicWidget *widget);
    static void saveMusiclist(QString name, MusicTableWidget *widget);
    static void saveLocalMusiclist(LocalMusicWidget *widget);
    static void savelists(QList<QString> lists);
    static void saveMusicPool(QHash<int,Music*> musicPool);
    static QList<Music *> fromIDlist(QHash<int, Music *> musicpool, QList<int> idlist);
};

#endif // DATAMANAGER_H
