#ifndef MUSICPOOL_H
#define MUSICPOOL_H

#include "music.h"

class MusicPool
{
public:
    MusicPool(QString name = "defaultMusicPool");
    ~MusicPool();

    void addMusic(Music *music);
    Music* getMusicByID(int ID);
    int getID(Music *music);
    int size();
    void save();
    void load();

private:
    QHash<int, Music*> list;
    QString name;
};


#endif // MUSICPOOL_H
